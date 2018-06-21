#!/usr/bin/env python3

import argparse
import collections
import glob
import json
import logging
import os
import re
import subprocess
import sys
import tempfile

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger
from util.console import query_yes_no


logger = get_logger('bench')


BASE_DIR = os.path.abspath(os.path.join(sys.path[0], '..'))
BENCHMARK_DIR = os.path.join(BASE_DIR, 'benchmarks')
CONFIG_DIR = os.path.join(BASE_DIR, 'configs')

ENV_FILE = os.path.join(CONFIG_DIR, 'env')
ENV_EXAMPLE_FILE = os.path.join(CONFIG_DIR, 'env.example')


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
        logging.disable(logging.DEBUG)  # we want to set all loggers

        harness = BenchmarkingHarness(BENCHMARK_DIR)

        add_default_runtimes(harness)

        harness.print_runtimes()

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
    def default_clean(workdir, **kwargs):
        assert os.path.isdir(workdir)

        # clean directory
        logger.info('clean benchmark directory')
        with subprocess.Popen(['make', 'clean'], cwd=workdir, stdout=subprocess.DEVNULL) as process:
            process.communicate()

            if process.returncode != 0:
                logger.error('"make clean" exited with non zero return code!')
                return False

        return True

    @staticmethod
    def default_make(workdir, make_env, **kwargs):
        assert os.path.isdir(workdir)

        make_params = ['-j', str(kwargs.get('make_jobs', 1))]  # -j = number of jobs to run simultaneously

        if kwargs.get('ignore_errors', False):
            make_params.append('-i')  # -i = ignore-errors
            logger.warning('errors during "make" will be ignored')

        for key in make_env:
            make_params.append('{}={}'.format(key, make_env[key]))

        logger.info('build benchmarks with "%s"', ' '.join(make_params))
        with subprocess.Popen(['make'] + make_params, cwd=args.testdir) as process:
            process.communicate()

            if process.returncode != 0:
                logger.error('"make" exited with non zero return code!')
                return False

        return True

    @staticmethod
    def default_executor(filepath, workdir, **kwargs):
        assert os.path.isfile(filepath)
        assert os.path.isdir(workdir)

        args = [filepath, '--output=json'] + kwargs.get('exec_args', '').split(' ')
        with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE) as process:
            stdout, _ = process.communicate(timeout=kwargs.get('timeout', 240))

            if process.returncode != 0:
                return None

            try:
                return json.loads(stdout.decode('utf-8'))
            except json.JSONDecodeError:
                logger.error('invalid benchmark result: \'%s\'', stdout.decode('utf-8'))
                raise

    def add_runtime(self, name, make_env,
                    make_func=default_make.__get__(object),
                    exec_func=default_executor.__get__(object),
                    clean_func=default_clean.__get__(object)):
        assert name not in self.registered_runtimes
        assert 'CC' in make_env and 'AS' in make_env

        logger.debug('register runtime to benchmark harness: "%s"', name)
        self.registered_runtimes[name] = {'make_env': make_env,  # enviroment variables required for make
                                          'make_func': make_func,  # function executed to build all benchmarks
                                          'exec_func': exec_func,  # function to execute a single benchmark
                                          'clean_func': clean_func}  # function executed after running all benchmarks

    def print_runtimes(self):
        print()
        print(" {:<30} | {}".format('runtime enviroment', 'enviroment variables'))
        print("{:-<32}|{:-<50}".format('', ''))
        for runtime, values in self.registered_runtimes.items():
            print(" {:<30} | {}".format(runtime, values['make_env']))
        print("{:-<32}|{:-<50}".format('', ''))

    def find_all_harness(self):
        found_harness = []
        for harness in sorted(os.listdir(self.testdir)):
            if harness.endswith("_test"):
                found_harness.append(harness)
        return found_harness

    def execute_runtimes(self, filter, bench_results, **kwargs):
        no_failures = True
        for runtime in self.registered_runtimes.keys():
            if re.match(filter, runtime):
                if self.execute_single_runtime(runtime, bench_results, **kwargs) is not True:
                    logger.error("%s did not finish", runtime)
                    no_failures = False

        return no_failures

    def execute_single_runtime(self, runtime, bench_results, **kwargs):
        assert type(bench_results) is BenchmarkingResults

        if runtime not in self.registered_runtimes:
            logger.error('runtime not found: "%s"', runtime)
            return {}

        runtime_name = runtime + kwargs.get('suffix', '')

        found_runtime = self.registered_runtimes[runtime]

        make_func = found_runtime['make_func']
        exec_func = found_runtime['exec_func']
        clean_func = found_runtime['clean_func']

        only_missing = kwargs.get('only_missing', False)

        logger.info('start with the execution of the runtime enviroment "%s"', runtime)

        try:
            # clean step
            if kwargs.get('skip_clean', False):
                logger.warning('clean step skipped for "%s"', runtime)
            else:
                logger.info('execute clean step for "%s"', runtime)
                if not clean_func(self.testdir, **kwargs):
                    logger.error('clean step for "%s" failed', runtime)
                    return False

            # compilation step
            if kwargs.get('skip_compilation', False):
                logger.warning('compilation step skipped for "%s"', runtime)
            else:
                logger.info('execute compilation step for "%s"', runtime)
                if not make_func(self.testdir, found_runtime['make_env'], **kwargs):
                    logger.error('compilation step for "%s" failed', runtime)
                    return False

            # execution step
            for run_id in range(kwargs.get('runs', 1)):
                logger.info('run %d of %d with the runtime enviroment "%s"', run_id+1, kwargs.get('runs', 1), runtime)
                for harness in self.find_all_harness():
                    harness_name = harness[:-len('_test')] if harness.endswith('_test') else harness
                    harness_path = os.path.join(self.testdir, harness)

                    if not re.match(kwargs.get('filter_harness', '.*'), harness):
                        logger.debug('benchmark: "%s:%s" did not match filter rule', harness_name, runtime_name)
                        continue

                    if only_missing and bench_results.is_run_present(harness_name, runtime_name):
                        logger.warning('benchmark run of "%s:%s" already present, skip', harness_name, runtime_name)
                        continue

                    try:
                        logger.info('benchmark: "%s:%s"', harness_name, runtime_name)
                        result_data = exec_func(harness_path, self.testdir, **kwargs)
                        if type(result_data) is dict and len(result_data) != 0:
                            # cleanup database entries
                            if kwargs.get('replace_runs'):
                                if run_id == 0:
                                    bench_results.remove_all_runs(harness_name, runtime_name)

                                bench_results.set_single_run(harness_name, runtime_name, result_data,
                                                             overwrite=True, run_id=run_id)
                            else:
                                bench_results.append_single_run(harness_name, runtime_name, result_data)
                        else:
                            logger.error('benchmark run of "%s:%s" did not return valid data: "%s"',
                                         harness_name, runtime_name, result_data)
                    except KeyboardInterrupt:
                        raise
                    except:  # NOQA: E722
                        logger.exception('Something went wrong while running the benchmark: "%s:%s"',
                                         harness_name, runtime_name)

        except KeyboardInterrupt:
            raise
        except:  # NOQA: E722
            logger.exception('Something went wrong while executing the runtime enviroment "%s"', runtime)
            return False

        return True


def add_default_runtimes(harness):
    """Those are some default runtimes which can also serve as example for custom ones"""

    if os.path.isfile(ENV_FILE):
        with open(ENV_FILE) as f:
            for line in f:
                key, value = line.rstrip().split('=', 1)
                os.environ[key] = value
    elif os.path.isfile(ENV_EXAMPLE_FILE):
        logger.warning('"%s" does not exist, use example file to specify required enviroment variables', ENV_FILE)
        with open(ENV_EXAMPLE_FILE) as f:
            for line in f:
                key, value = line.rstrip().split('=', 1)
                os.environ[key] = value
    else:
        logger.warning('no "env" file found in "%s"', CONFIG_DIR)

    def wllvm_make(workdir, make_env, **kwargs):
            if 'LLVM_COMPILER' not in os.environ:
                logger.error('enviroment variable "LLVM_COMPILER" is required to build with wllvm')
                return False

            return BenchmarkingHarness.default_make(workdir, make_env, **kwargs)

    def wllvm_executor(filepath, workdir, tool, **kwargs):
        assert os.path.isfile(filepath)
        assert os.path.isdir(workdir)

        with tempfile.TemporaryDirectory() as tmp:
            bc_filename = os.path.splitext(os.path.basename(filepath))[0] + '.bc'
            bc_filepath = os.path.join(tmp, bc_filename)
            logger.debug('extract bitcode file to: "%s"', bc_filepath)

            with subprocess.Popen([os.path.expandvars("$WLLVM_DIR/extract-bc"), filepath, '-o', bc_filepath]) as p:
                p.wait(timeout=30)  # 30 Seconds should be way enough time to do the bitcode extraction

            assert os.path.isfile(bc_filepath)

            additional_args = kwargs.get('exec_args', '').split(' ')
            warmup_iterations = kwargs.get('warmup_iterations', None)
            if warmup_iterations:
                additional_args.append('--warmup=%d' % warmup_iterations)
            args = os.path.expandvars(tool).split(' ') + [bc_filepath, '--output=json'] + additional_args
            with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE) as p:
                stdout, _ = p.communicate(timeout=kwargs.get('timeout', 240))

                if p.returncode != 0:
                    return None

                try:
                    return json.loads(stdout.decode('utf-8'))
                except json.JSONDecodeError:
                    logger.error('invalid benchmark result: \'%s\'', stdout.decode('utf-8'))
                    raise

    for config in glob.glob(os.path.join(CONFIG_DIR, '*.py')):
        with open(config) as f:
            exec(f.read(), {'wllvm_make': wllvm_make, 'wllvm_executor': wllvm_executor,
                            'harness': harness, 'logger': logger})


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Execute benchmarks and export them',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('benchfile', metavar='BENCHFILE', type=str,
                        help='file where the benchmarks are written to')

    parser.add_argument('--testdir', metavar='TESTDIR', type=_TestDirType(), default=BENCHMARK_DIR,
                        help='directory where the tests and the Makefile is contained')
    parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which runtimes should be used')
    parser.add_argument('--filter-harness', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which harness should be used (based on filename)')
    parser.add_argument('--timeout', metavar='SECONDS', type=int, default=240,
                        help='timeout of a single benchmark run, given in seconds')
    parser.add_argument('--suffix', metavar='STRING', type=str, default="",
                        help='suffix which should be added to the runtime name')
    parser.add_argument('--runs', metavar='COUNT', type=int, default=1,
                        help='number of times a benchmark should be repeated')
    parser.add_argument('--exec-args', metavar='STRING', type=str, default="",
                        help='commandline arguments which should be added to the benchmark')

    parser.add_argument('--only-missing', action='store_true',
                        help='only execute benchmarks which are missing in the benchfile')
    parser.add_argument('--replace-runs', action='store_true',
                        help='overwrite runs instead of appending to them')
    parser.add_argument('--list-runtimes', action=_ListAllRuntimeAction,
                        help='show a list of all runtimes which can be executed')
    # parser.add_argument('--no-color', action='store_true',
    #                    help='disable color output')
    parser.add_argument('--skip-clean', action='store_true',
                        help='skip the clean step when benchmarking')
    parser.add_argument('--skip-compilation', action='store_true',
                        help='skip the compilation step when benchmarking')
    parser.add_argument('--ignore-errors', '-i', action='store_true',
                        help='ignore all errors in the make step to do at least a partial benchmark')
    parser.add_argument('--jobs', '-j', type=int, default=2,
                        help='number of jobs used for make')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')
    parser.add_argument('--yes', '-y', action='store_true',
                        help='answer all questions with yes')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    if not is_system_ready_for_benchmarking():
        if not args.yes and query_yes_no('Do you want to continue with the benchmarks?') == 'no':
            sys.exit()

    # Initialize Benchmarking Harness
    harness = BenchmarkingHarness(args.testdir)

    add_default_runtimes(harness)

    results = BenchmarkingResults()

    if os.path.isfile(args.benchfile):
        try:
            with open(args.benchfile, 'r') as f:
                results.load_file(f)
        except Exception:
            logger.exception("cannot load file")
            corrupted_msg = 'Do you want to continue? The given file exists, but does not contain valid data'
            if not args.yes and query_yes_no(corrupted_msg, default="no") == 'no':
                sys.exit()

    no_failures = True
    try:
        execution_kwargs = {
            'skip_clean': args.skip_clean or args.skip_compilation,
            'skip_compilation': args.skip_compilation,
            'filter_harness': args.filter_harness,
            'ignore_errors': args.ignore_errors,
            'make_jobs': args.jobs,
            'timeout': args.timeout,
            'suffix': args.suffix,
            'runs': args.runs,
            'replace_runs': args.replace_runs,
            'exec_args': args.exec_args,
            'only_missing': args.only_missing
        }

        no_failures = harness.execute_runtimes(args.filter_runtime, results, **execution_kwargs)
    except KeyboardInterrupt:
        pass
    except:  # NOQA: E722
        no_failures = False
        logger.exception('Something went wrong while executing the testcases')
    finally:
        logger.info('write results into benchmark file')
        with open(args.benchfile, 'w') as f:
            results.store_file(f)

        sys.exit(0 if no_failures else 1)
