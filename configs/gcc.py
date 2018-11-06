from functools import partial
import os
import subprocess
import json

gcc_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('gcc-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0"}, **gcc_kwargs)
harness.add_runtime('gcc-O1', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O1"}, **gcc_kwargs)
harness.add_runtime('gcc-O2', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O2"}, **gcc_kwargs)
harness.add_runtime('gcc-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3"}, **gcc_kwargs)
harness.add_runtime('gcc-march-native-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -march=native"}, **gcc_kwargs)
harness.add_runtime('gcc-Ofast', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -Ofast"}, **gcc_kwargs)
harness.add_runtime('gcc-Os', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -Os"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-strict-aliasing-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-strict-aliasing"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-strict-overflow-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-strict-overflow"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-delete-null-pointer-checks-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-delete-null-pointer-checks"}, **gcc_kwargs)
harness.add_runtime('gcc-gcov-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0 -fprofile-arcs -ftest-coverage"}, **gcc_kwargs)
harness.add_runtime('gcc-gprof-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0 -pg"}, **gcc_kwargs)
harness.add_runtime('gcc-mpx-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": " -std=gnu99 -O3 -mmpx -fcheck-pointer-bounds -lmpxwrappers"}, **gcc_kwargs)


# get line number coverage

# todo: duplicated from bench.py
def default_executor(filepath, workdir, exec_args, **kwargs):
        assert os.path.isfile(filepath)
        assert os.path.isdir(workdir)

        args = [filepath, '--output=json'] + exec_args
        env = os.environ.copy()

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


def execute_gcov(filepath, workdir, exec_args, **kwargs):
    with subprocess.Popen(['gcov', filepath, '-o', workdir, '-i'], cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
        stdout, stderr = process.communicate(timeout=kwargs.get('timeout', 240))
        stdout_decoded = stdout.decode('utf-8') if stdout else None
    return stdout_decoded


def gcov_executor(filepath, workdir, exec_args, **kwargs):
    json, _ = default_executor(filepath, workdir, exec_args)  # execute file
    stderr_decoded = execute_gcov(filepath, workdir, exec_args, **kwargs)
    import glob
    assert(filepath.endswith('_test'))
    benchmark_dir_name = filepath[:-5]
    for filename in glob.iglob(benchmark_dir_name + '/**/*.c', recursive=True):
        print('filename: ' + filename)
        gcov_info = execute_gcov(filename, workdir, exec_args, **kwargs)
        if gcov_info is not None:
            stderr_decoded += gcov_info
    return json, stderr_decoded


gcc_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version'), 'exec_func': gcov_executor}

harness.add_runtime('gcc-gcov-line-numbers', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-g -std=gnu99 -O0 -fprofile-arcs -ftest-coverage", "LDFLAGS": "-ftest-coverage"}, **gcc_kwargs)
