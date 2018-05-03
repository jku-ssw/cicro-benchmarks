#!/usr/bin/env python3

import argparse
import collections
import glob
import logging
import json
import os
import subprocess
import sys
import tempfile

from util.color_logger import get_logger
from util.console import query_yes_no


logger = get_logger('bench' )


class _TestDirType(object):
    def __init__(self):
        """Checks if the given directory exists and contains a Makefile"""
        pass

    def __call__(self, string):
        if not os.path.exists(string):
            raise argparse.ArgumentTypeError("path does not exist: '%s'".format(string))
        if not os.path.isdir(string):
            raise argparse.ArgumentTypeError("path is not a directory:'%s'".format(string))
        if not os.path.isfile(os.path.join(string, 'Makefile')):
            raise argparse.ArgumentTypeError("Makefile not found in: '%s'".format(string))

        return string


class _ListAllRuntimeAction(argparse._StoreTrueAction):
    def __init__(self, option_strings, dest, **kwargs):
        """Lists all available runtime and then exit"""
        super(_ListAllRuntimeAction, self).__init__(option_strings, dest, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        raise NotImplementedError("List Runtime is not implemented yet!")  # TODO: implement
        parser.exit()


def is_system_ready_for_benchmarking():
    """check if the system is ready for benchmarking"""

    # check if scaling_governor is set to 'performance' for all cpu cores
    cpu_governors = glob.glob('/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor')
    if not cpu_governors:
        logger.error('no scaling_governor found. Do you run on a Linux System?')
        return False
    for governor in sorted(cpu_governors):
        with open(governor, 'r') as f:
            line = f.read().splitlines()[0]
            logger.debug('%s is set to \"%s\"', governor, line)
            if line != 'performance':
                logger.warning('please set all scaling_governor to \"performance\" (using "sudo ./ondemand.sh start")')
                return False

    return True


class BenchmarkingHarness(object):
    def __init__(self, testdir):
        assert os.path.isdir(testdir)

        self.testdir = os.path.abspath(testdir)  # Just to be sure, work with absolute paths

        self.registered_runtimes = collections.OrderedDict()

    @staticmethod
    def default_make(workdir, make_env, **kwargs):
        # clean directory
        logger.info('clean benchmark directory')
        process = subprocess.Popen(['make', 'clean'], cwd=workdir, stdout=subprocess.DEVNULL)
        process.communicate()

        make_params = []
        for key in make_env:
            make_params.append('{}={}'.format(key, make_env[key]))

        # build all tests
        logger.info('build benchmarks with "%s"', " ".join(make_params))
        process = subprocess.Popen(['make', '-i'] + make_params, cwd=args.testdir)  # -i = ignore single errors
        process.communicate()

    @staticmethod
    def default_executor(filepath, workdir, **kwargs):
        pass

    @staticmethod
    def default_cleanup(workdir, **kwargs):
        pass

    def add_runtime(self, name, make_env, make_func=default_make.__get__(object), exec_func=default_executor.__get__(object), cleanup_func=default_cleanup.__get__(object)):
        assert name not in self.registered_runtimes
        assert 'CC' in make_env and 'AS' in make_env

        logger.debug('register runtime to benchmark harness: "%s"', name)
        self.registered_runtimes[name] = {'make_env': make_env,  # enviroment variables required for make
                                          'make_func': make_func,  # function executed to build all benchmarks
                                          'exec_func': exec_func,  # function executed for every benchmark, to be executed
                                          'cleanup_func': cleanup_func}  # function executed after running all benchmarks

    def print_runtimes(self):
        print()
        print(" {:<30} | {}".format('runtime enviroment', 'enviroment variables'))
        print("{:-<32}|{:-<50}".format('', ''))
        for runtime in self.registered_runtimes:
            values = self.registered_runtimes[runtime]
            print(" {:<30} | {}".format(runtime, values['make_env']))
        print("{:-<32}|{:-<50}".format('', ''))

    def execute_runtimes(self, regex):
        pass

    def execute_single_runtime(self, runtime):
        if runtime not in self.registered_runtimes:
            logger.error('runtime not found: "%s"', runtime)
            return {}

        found_runtime = self.registered_runtimes[runtime]

        make_func = found_runtime['make_func']
        exec_func = found_runtime['exec_func']
        cleanup_func = found_runtime['cleanup_func']


        try:
            make_func(self.testdir, found_runtime['make_env'])
        except:
            logger.exception('Something went wrong while building the benchmarks')
            return {}

def add_default_runtimes(harness):
    """Those are some default runtimes which can also serve as example for custom ones"""

    # Clang
    harness.add_runtime('clang-O0', {"CC": "clang", "AS": "clang"})
    harness.add_runtime('clang-O1', {"CC": "clang", "AS": "clang", "CFLAGS": "-O1"})
    harness.add_runtime('clang-O2', {"CC": "clang", "AS": "clang", "CFLAGS": "-O2"})
    harness.add_runtime('clang-O3', {"CC": "clang", "AS": "clang", "CFLAGS": "-O3"})

    # Clang + AddressSanitizer
    harness.add_runtime('clang-fsanitize=address-O0', {"CC": "clang", "AS": "clang"})
    harness.add_runtime('clang-fsanitize=address-O1', {"CC": "clang", "AS": "clang", "CFLAGS": "-O1 -fsanitize=address", "LDFLAGS": "-fsanitize=address"})
    harness.add_runtime('clang-fsanitize=address-O2', {"CC": "clang", "AS": "clang", "CFLAGS": "-O2 -fsanitize=address -Wno-everything", "LDFLAGS": "-fsanitize=address"})
    harness.add_runtime('clang-fsanitize=address-O3', {"CC": "clang", "AS": "clang", "CFLAGS": "-O3 -fsanitize=address", "LDFLAGS": "-fsanitize=address"})

    # lli (bytecode executor)
    def lli_executor(filepath, workdir, **kwargs):
        with tempfile.TemporaryDirectory() as tmp:
            bc_filename = os.path.splitext(os.path.basename(filepath))[0] + '.bc'
            bc_filepath = os.path.join(tmp, bc_filename)
            logger.debug('extract bitcode file to: "%s"', bc_filepath)

            process_bc = subprocess.Popen(["extract-bc", filepath, '-o', bc_filepath])
            process_bc.wait(timeout=30)  # 30 Seconds should be way enough time to do the bitcode extraction

            # TODO: execute testcase
            raise NotImplementedError("execution of testcases is not implemented yet")

    harness.add_runtime('lli-O0', {"CC": "wllvm", "AS": "wllvm"}, exec_func=lli_executor)
    harness.add_runtime('lli-O1', {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O1"}, exec_func=lli_executor)
    harness.add_runtime('lli-O2', {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O2"}, exec_func=lli_executor)
    harness.add_runtime('lli-O3', {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O3"}, exec_func=lli_executor)

    # GNU Compiler Collection
    harness.add_runtime('gcc-O0', {"CC": "gcc", "AS": "as"})
    harness.add_runtime('gcc-O1', {"CC": "gcc", "AS": "as", "CFLAGS": "-O1"})
    harness.add_runtime('gcc-O2', {"CC": "gcc", "AS": "as", "CFLAGS": "-O2"})
    harness.add_runtime('gcc-O3', {"CC": "gcc", "AS": "as", "CFLAGS": "-O3"})

    # Tiny C Compiler
    harness.add_runtime('tcc-O0', {"CC": "tcc", "AS": "tcc"})
    harness.add_runtime('tcc-O1', {"CC": "tcc", "AS": "tcc", "CFLAGS": "-O1"})
    harness.add_runtime('tcc-O2', {"CC": "tcc", "AS": "tcc", "CFLAGS": "-O2"})
    harness.add_runtime('tcc-O3', {"CC": "tcc", "AS": "tcc", "CFLAGS": "-O3"})


if __name__ == "__main__":
    # Parse Comandline Arguments
    parser = argparse.ArgumentParser(description='Execute benchmarks and export them')

    parser.add_argument('testdir', metavar='TESTDIR', type=_TestDirType(),
                        help='directory where the tests and the Makefile is contained')
    parser.add_argument('benchfile',metavar='BENCHFILE', type=argparse.FileType('w'),
                        help='file where the benchmarks are written to')

    parser.add_argument('--filter-runtimes', metavar='REGEX', type=str, default='gcc-O0|clang-O0',
                        help='regular expression to select which runtimes shold be used')
    parser.add_argument('--timeout', metavar='SECONDS', type=int, default=60,
                        help='timeout of a single benchmark run, given in seconds')
    parser.add_argument('--suffix', metavar='STRING', type=str, default="",
                        help='suffix which should be added to the runtime name')

    parser.add_argument('--only-missing', action='store_true',
                        help='only execute benchmarks which are missing in the benchfile')
    parser.add_argument('--list-runtimes', action=_ListAllRuntimeAction,
                        help='show a list of all runtimes which can be executed')
    #parser.add_argument('--no-color', action='store_true',
    #                    help='disable color output')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')
    parser.add_argument('--yes', '-y', action='store_true',
                        help='answer all questions with yes')

    args = parser.parse_args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)

    if not is_system_ready_for_benchmarking():
        if not args.yes and query_yes_no('Do you want to continue with the benchmarks?') == 'no':
            sys.exit()

    # Initialize Benchmarking Harness
    harness = BenchmarkingHarness(args.testdir)

    add_default_runtimes(harness)

    harness.print_runtimes()

    harness.execute_single_runtime('clang-fsanitize=address-O2')