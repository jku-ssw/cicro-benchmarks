#!/usr/bin/env python3

import argparse
import collections
import glob
import json
import logging
import os
import platform
import re
import subprocess
import sys
import tempfile

from datetime import datetime, timezone

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger
from util.console import query_yes_no


logger = get_logger('bench')


try:
    import psutil
    psutil_imported = True
except ImportError:
    psutil_imported = False
    logger.warning("psutil not installed, will skip system related metadata")


BASE_DIR = os.path.abspath(os.path.join(sys.path[0], '..'))
BENCHMARK_DIR = os.path.join(BASE_DIR, 'benchmarks')
CONFIG_DIR = os.path.join(BASE_DIR, 'configs')

ENV_FILE = os.path.join(CONFIG_DIR, 'env')
ENV_RUN_FILE = os.path.join(CONFIG_DIR, 'env.run')
run_env = {}
ENV_EXAMPLE_FILE = os.path.join(CONFIG_DIR, 'env.example')


class _DirType(object):
    def __init__(self):
        """Checks if the given directory exists"""
        pass

    def __call__(self, string):
        if not os.path.exists(string):
            raise argparse.ArgumentTypeError("path does not exist: '%s'".format(string))
        if not os.path.isdir(string):
            raise argparse.ArgumentTypeError("path is not a directory:'%s'".format(string))

        return string


class _TestDirType(_DirType):
    def __init__(self):
        """Checks if the given directory exists and contains a Makefile"""
        super(_TestDirType, self).__init__()

    def __call__(self, string):
        super(_TestDirType, self).__call__(string)
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
                logger.warning('please set all scaling_governor to "performance" '
                               '(using "sudo ./tools/ondemand.sh start")')
                return False

    return True


def is_papi_installed():
    """Check if PAPI is installed for performance counter measurements"""

    with subprocess.Popen(('ldconfig', '-p'), stdout=subprocess.PIPE) as p:
        with subprocess.Popen(('grep', 'libpapi.so'), stdin=p.stdout, stdout=subprocess.PIPE) as pg:
            stdout, _ = pg.communicate()
            return len(stdout) != 0


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

        for key in make_env:
            make_params.append('{}={}'.format(key, make_env[key]))

        with subprocess.Popen(['make', "../C-Hayai/build/src/libchayai.a"] + make_params, cwd=args.testdir) as process:
            process.communicate()

            if process.returncode != 0:
                logger.error('"make" of benchmark harness exited with non zero return code!')
                return False

        if kwargs.get('ignore_errors', False):
            make_params.append('-i')  # -i = ignore-errors
            logger.warning('errors during "make" will be ignored and a 0 exit code will be returned')

        if 'make_target' in  kwargs:
            make_params.append(kwargs['make_target'])

        logger.info('build benchmarks with "%s"', ' '.join(make_params))
        with subprocess.Popen(['make'] + make_params, cwd=args.testdir) as process:
            process.communicate()

            if process.returncode != 0:
                logger.error('"make" exited with non zero return code!')
                return False

        # check if all benchmarks were compiled (important if we ignore errors)
        dir_content = os.listdir(workdir)
        orig_test_files = [elem for elem in dir_content if elem.endswith('_test.c')]
        for test_file in sorted(orig_test_files):
            if test_file[:-2] not in dir_content:
                logger.warning('%s was not compiled!', test_file)

        return True

    @staticmethod
    def default_executor(filepath, workdir, exec_args, **kwargs):
        assert os.path.isfile(filepath)
        assert os.path.isdir(workdir)

        args = [filepath, '--output=json'] + exec_args
        env = os.environ.copy()
        for key, val in run_env.items():
            env[key] = val

        with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env) as process:
            stdout, stderr = process.communicate(timeout=kwargs.get('timeout', 240))

            stdout_decoded = stdout.decode('utf-8') if stdout else None
            stderr_decoded = stderr.decode('utf-8') if stderr else None

            if stdout_decoded:
                try:
                    return json.loads(stdout_decoded), stderr_decoded
                except ValueError:
                    logger.exception('invalid benchmark result: \'%s\'', stdout_decoded)

            return None, stderr_decoded

    def add_runtime(self, name, make_env,
                    build_system_func=None,
                    make_func=default_make.__get__(object),
                    exec_func=default_executor.__get__(object),
                    clean_func=default_clean.__get__(object)):
        assert name not in self.registered_runtimes
        assert 'CC' in make_env and 'AS' in make_env

        logger.debug('register runtime to benchmark harness: "%s"', name)
        self.registered_runtimes[name] = {'make_env': make_env,  # enviroment variables required for make
                                          'build_system_func': build_system_func,  # function to get system informations
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

    def filtered_runtime_iterator(self, filter):
        for runtime in self.registered_runtimes.keys():
            if re.match(filter, runtime):
                yield runtime

    def execute_runtimes(self, filter, bench_results, **kwargs):
        no_failures = True
        for runtime in self.filtered_runtime_iterator(filter):
            if self.execute_single_runtime(runtime, bench_results, **kwargs) is not True:
                logger.error('runtime "%s" did not finish successful', runtime)
                no_failures = False

        return no_failures

    def execute_single_runtime(self, runtime, bench_results, **kwargs):
        assert type(bench_results) is BenchmarkingResults

        if runtime not in self.registered_runtimes:
            logger.error('runtime not found: "%s"', runtime)
            return False

        runtime_name = runtime + kwargs.get('suffix', '')

        found_runtime = self.registered_runtimes[runtime]
        make_env = found_runtime.get('make_env', {}).copy()

        if kwargs['no_papi']:
            make_env["PAPI"] = 0

        kwargs['make_env_copy'] = make_env  # when we need to compile something in the execution again (PGO)

        exec_kwargs = kwargs.copy()
        del exec_kwargs['exec_args']

        build_system_func = found_runtime['build_system_func']
        make_func = found_runtime['make_func']
        exec_func = found_runtime['exec_func']
        clean_func = found_runtime['clean_func']

        result_writer_func = kwargs['result_writer_func']

        only_missing = kwargs.get('only_missing', False)

        no_error = True

        logger.info('start with the execution of the runtime enviroment "%s"', runtime)

        result_harness_data = {
            'system': {'platform': platform.platform(True),
                       'cpu': {'cores_logical': os.cpu_count()}}}

        if psutil_imported:
            try:
                result_harness_data['system']['cpu']['cores_physical'] = psutil.cpu_count(logical=False)
            except KeyboardInterrupt:
                raise
            except:  # NOQA: E722
                logger.exception('cannot extract system informations with psutil')

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
                if not make_func(self.testdir, make_env, **kwargs):
                    logger.error('compilation step for "%s" failed', runtime)
                    return False

                result_harness_data['make_env'] = {}
                for key, value in make_env.items():
                    exp_value = os.path.expandvars(value) if type(value) is str else value
                    result_harness_data['make_env'][key] = exp_value

                if build_system_func:
                    try:
                        # Use our expanded enviroment variables
                        build_system_data = build_system_func(result_harness_data.get('make_env', {}))
                        if build_system_data:
                            logger.debug('build system extracted: %s', build_system_data)
                            result_harness_data['build_system'] = build_system_data
                    except KeyboardInterrupt:
                        raise
                    except:  # NOQA: E722
                        logger.exception('cannot extract build system informations')

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

                    # get some additional statistics which could be of interest
                    if psutil_imported:
                        try:
                            result_harness_data['system']['cpu']['percent'] = psutil.cpu_percent(interval=0.5,
                                                                                                 percpu=True)
                            result_harness_data['system']['cpu']['freq'] = psutil.cpu_freq(percpu=True)

                            virtual_memory = psutil.virtual_memory()
                            result_harness_data['system']['memory'] = {'total': virtual_memory.total,
                                                                       'available': virtual_memory.available,
                                                                       'used': virtual_memory.used,
                                                                       'free': virtual_memory.free}
                        except KeyboardInterrupt:
                            raise
                        except:  # NOQA: E722
                            logger.exception('cannot extract system informations with psutil')

                    result_harness_data['datetime'] = datetime.now(timezone.utc).astimezone().isoformat()

                    try:
                        exec_args = [x for x in kwargs.get('exec_args', '').split(' ') if x]

                        iteration_overwrite = kwargs.get('iterdata', {}).get(harness + ".c")
                        if iteration_overwrite is not None:
                            if type(iteration_overwrite) is int:
                                exec_args.append("--iterations={}".format(iteration_overwrite))
                            else:
                                logger.error('"%s" is not of type int, ignore iteration overwrite', iteration_overwrite)

                        logger.info('benchmark: "%s:%s"', harness_name, runtime_name)
                        if exec_args:
                            logger.info(' * additional args set: "%s"', " ".join(exec_args))

                        exec_ret = exec_func(harness_path, self.testdir, exec_args, **exec_kwargs)

                        # either we only return the data, or stderr as second element
                        if type(exec_ret) is tuple:
                            result_data = exec_ret[0]
                            result_stderr = exec_ret[1]

                            if result_stderr:
                                logger.warning("benchmark harness had some output on stderr:\n%s", result_stderr)
                                result_harness_data['stderr'] = result_stderr
                                no_error = False
                                if kwargs.get('ignore_invalid_measurements', False) and result_data:
                                    for benchmark in result_data.get('benchmarks', []):
                                        logger.debug("remove data from measurement:\n%s", benchmark)
                                        if 'runs' in benchmark:
                                            benchmark['runs'] = []
                                        if 'mean' in benchmark:
                                            benchmark['mean'] = None
                                        if 'std_dev' in benchmark:
                                            benchmark['std_dev'] = None
                        else:
                            result_data = exec_ret

                        if (type(result_data) is dict and len(result_data) != 0) or 'stderr' in result_harness_data:
                            # cleanup database entries
                            if kwargs.get('replace_runs'):
                                if run_id == 0:
                                    bench_results.remove_all_runs(harness_name, runtime_name)

                                bench_results.set_single_run(harness_name, runtime_name, result_data,
                                                             harness_data=result_harness_data,
                                                             overwrite=True, run_id=run_id)
                            else:
                                bench_results.append_single_run(harness_name, runtime_name, result_data,
                                                                harness_data=result_harness_data)
                        else:
                            logger.error('benchmark run of "%s:%s" did not return valid data: "%s"',
                                         harness_name, runtime_name, result_data)
                            no_error = False
                    except KeyboardInterrupt:
                        raise
                    except:  # NOQA: E722
                        logger.exception('Something went wrong while running the benchmark: "%s:%s"',
                                         harness_name, runtime_name)
                        no_error = False

                # write results after every run
                if run_id + 1 < kwargs.get('runs', 1):
                    try:
                        result_writer_func()
                    except KeyboardInterrupt:
                        raise
                    except:  # NOQA: E722
                        logger.exception('Something went wrong while writing the results')
                        no_error = False

        except KeyboardInterrupt:
            raise
        except:  # NOQA: E722
            logger.exception('Something went wrong while executing the runtime enviroment "%s"', runtime)
            return False
        finally:
            try:
                result_writer_func()
            except KeyboardInterrupt:
                raise
            except:  # NOQA: E722
                logger.exception('Something went wrong while writing the results')

        return no_error


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
    if os.path.isfile(ENV_RUN_FILE):
        with open(ENV_RUN_FILE) as f:
            for line in f:
                key, value = line.rstrip().split('=', 1)
                run_env[key] = value

    def wllvm_make(workdir, make_env, **kwargs):
            if 'LLVM_COMPILER' not in os.environ:
                logger.error('enviroment variable "LLVM_COMPILER" is required to build with wllvm')
                return False

            return BenchmarkingHarness.default_make(workdir, make_env, **kwargs)

    def wllvm_executor(filepath, workdir, tool, exec_args, **kwargs):
        assert os.path.isfile(filepath)
        assert os.path.isdir(workdir)

        with tempfile.TemporaryDirectory() as tmp:
            bc_filename = os.path.splitext(os.path.basename(filepath))[0] + '.bc'
            bc_filepath = os.path.join(tmp, bc_filename)
            logger.debug('extract bitcode file to: "%s"', bc_filepath)

            with subprocess.Popen([os.path.expandvars("${WLLVM_DIR}/extract-bc"), filepath, '-o', bc_filepath]) as p:
                p.wait(timeout=30)  # 30 Seconds should be way enough time to do the bitcode extraction

            assert os.path.isfile(bc_filepath)

            args = os.path.expandvars(tool).split(' ') + [bc_filepath, '--output=json'] + exec_args
            with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as p:
                stdout, stderr = p.communicate(timeout=kwargs.get('timeout', 240))

                stdout_decoded = stdout.decode('utf-8') if stdout else None
                stderr_decoded = stderr.decode('utf-8') if stderr else None

                if stdout_decoded:
                    try:
                        return json.loads(stdout_decoded), stderr_decoded
                    except ValueError:
                        logger.exception('invalid benchmark result: \'%s\'', stdout_decoded)

                return None, stderr_decoded

    def build_system_executor(make_env, cc_version, as_version):
        result = {}

        my_env = os.environ.copy()
        my_env['LC_ALL'] = 'C'  # Force results in default language

        if 'CC' in make_env and cc_version:
            with subprocess.Popen([make_env['CC'], cc_version], stdout=subprocess.PIPE, env=my_env) as p:
                stdout, _ = p.communicate(timeout=1)

                stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
                if p.returncode == 0 and stdout_decoded:
                    result['CC_version'] = stdout_decoded

        if 'AS' in make_env and as_version:
            with subprocess.Popen([make_env['AS'], cc_version], stdout=subprocess.PIPE, env=my_env) as p:
                stdout, _ = p.communicate(timeout=1)

                stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
                if p.returncode == 0 and stdout_decoded:
                    result['AS_version'] = stdout_decoded

        return result

    for config in glob.glob(os.path.join(CONFIG_DIR, '*.py')):
        with open(config) as f:
            exec(f.read(), {'default_clean': BenchmarkingHarness.default_clean,
                            'default_make': BenchmarkingHarness.default_make,
                            'default_executor': BenchmarkingHarness.default_executor,
                            'wllvm_make': wllvm_make,
                            'wllvm_executor': wllvm_executor,
                            'build_system_executor': build_system_executor,
                            'harness': harness, 'logger': logger, 'run_env': run_env})


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Execute benchmarks and export them',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=40,
                                                                                         width=120))

    parser.add_argument('benchfile', metavar='BENCHFILE', type=str,
                        help='file where the benchmarks are written to')

    parser.add_argument('--testdir', metavar='TESTDIR', type=_TestDirType(), default=BENCHMARK_DIR,
                        help='directory where the tests and the Makefile is contained')
    parser.add_argument('--workdir', metavar='TESTDIR', type=_DirType(), default=None,
                        help='directory where temporary files will be stored')
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
    parser.add_argument('--iterfile', metavar='ITERFILE', nargs='?', type=argparse.FileType('r'),
                        help='file which overwrites the number of iterations per harness')

    parser.add_argument('--no-papi', action='store_true',
                        help='do not compile performance counter library into benchmarks')
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
                        help='ignore all errors in the make step and run only the successfully compiled benchmarks. '
                             'Furthermore return a 0 exit code even when some errors occured')
    parser.add_argument('--ignore-invalid-measurements', '-im', action='store_true',
                        help='ignore measurements of runs which exited with an error code')
    parser.add_argument('--jobs', '-j', type=int, default=int(os.cpu_count()/2) + 1,
                        help='number of jobs used for make')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')
    parser.add_argument('--yes', '-y', action='store_true',
                        help='answer all questions with yes')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    if not args.no_papi and not is_papi_installed():
        logger.warning('PAPI does not seem to be installed on your system. '
                       'You can exclude performance counter support with "--no-papi"')

    # Initialize Benchmarking Harness
    harness = BenchmarkingHarness(args.testdir)

    add_default_runtimes(harness)

    runtimes_to_execute = list(harness.filtered_runtime_iterator(args.filter_runtime))
    if len(runtimes_to_execute) == 0:
        logger.error('no runtimes found which is matching the filter: "%s"', args.filter_runtime)
        sys.exit(1)
    else:
        logger.info('Following runtimes will be executed:')
        for runtime in runtimes_to_execute:
            logger.info(' * %s', runtime)

    results = BenchmarkingResults()

    if args.workdir:
        os.environ["CHAYAI_WORKDIR"] = os.path.abspath(args.workdir)

    if os.path.isfile(args.benchfile):
        try:
            with open(args.benchfile, 'r') as f:
                results.load_file(f)
        except Exception:
            logger.exception("cannot load file")
            corrupted_msg = 'Do you want to continue? The given file exists, but does not contain valid data'
            if not args.yes and query_yes_no(corrupted_msg, default="no", on_keyboard_int="no") == 'no':
                sys.exit()

    if args.iterfile:
        logger.warning("benchmark iteration count will be forced to your specified value")
        iterdata = json.load(args.iterfile)
        logger.info(iterdata)
    else:
        iterdata = {}

    def write_results():
        """ atomic write of results into a file"""
        logger.info('write results into benchmark file')
        try:
            with tempfile.NamedTemporaryFile('w', dir=os.path.dirname(args.benchfile),
                                             prefix=".bench_", suffix='.json', delete=False) as f:
                # write results
                results.store_file(f)
                os.fsync(f.fileno())

                # if the file exists, move it somewhere else
                tmp_filename = args.benchfile + '.deleteme'
                delete_tmp_file = False
                if os.path.exists(args.benchfile):
                    assert not os.path.exists(tmp_filename)
                    os.link(args.benchfile, tmp_filename)
                    os.unlink(args.benchfile)
                    delete_tmp_file = True

                # link new file
                os.link(f.name, args.benchfile)
                os.unlink(f.name)

                # remove old file
                if delete_tmp_file:
                    os.unlink(tmp_filename)
        except FileNotFoundError as e:  # NOQA: F821
            logger.warning('use fallback method to write file, because atomic function failed')
            logger.debug(str(e))
            with open(args.benchfile, 'w') as f:
                # write results directly into result file
                results.store_file(f)
                os.fsync(f.fileno())

    execution_kwargs = {
        'skip_clean': args.skip_clean or args.skip_compilation,
        'skip_compilation': args.skip_compilation,
        'filter_harness': args.filter_harness,
        'ignore_errors': args.ignore_errors,
        'ignore_invalid_measurements': args.ignore_invalid_measurements,
        'make_jobs': args.jobs,
        'timeout': args.timeout,
        'suffix': args.suffix,
        'runs': args.runs,
        'replace_runs': args.replace_runs,
        'exec_args': args.exec_args,
        'iterdata': iterdata,
        'no_papi': args.no_papi,
        'only_missing': args.only_missing,
        'result_writer_func': write_results
    }

    if not is_system_ready_for_benchmarking():
        if not args.yes and query_yes_no('Do you want to continue with the benchmarks?', on_keyboard_int="no") == 'no':
            sys.exit()

    no_failures = True
    try:
        no_failures = harness.execute_runtimes(args.filter_runtime, results, **execution_kwargs)
    except KeyboardInterrupt:
        pass
    except:  # NOQA: E722
        no_failures = False
        logger.exception('Something went wrong while executing the testcases')
    finally:
        # write_results()  # writing to file now happens in execute_single_runtime(...)
        if args.ignore_errors and not no_failures:
            logger.warning('Ignored failed run and return 0 as exit code')
            sys.exit(0)
        sys.exit(0 if no_failures else 1)
