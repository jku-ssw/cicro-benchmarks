import json
import os
import subprocess


def valgrind_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen(['valgrind', '--version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['VALGRIND_version'] = stdout_decoded

    return result


def ltrace_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen(['ltrace', '-V'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['LTRACE_version'] = stdout_decoded

    return result


def strace_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen(['strace', '-V'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['STRACE_version'] = stdout_decoded

    return result


def drmemory_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen([os.environ['DR_MEMORY'], '-version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['DRMEMORY_version'] = stdout_decoded

    return result


def qemu_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen(['qemu-x86_64', '-version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['QEMU_version'] = stdout_decoded

    return result


def boehmgc_build_system_executor(make_env):
    return build_system_executor(make_env, cc_version='--version', as_version='--version')


def diehard_build_system_executor(make_env):
    return build_system_executor(make_env, cc_version='--version', as_version='--version')


def execute_binary_analysis_tool(filepath, workdir, tool, exec_args, env=None, **kwargs):
    env_tool = [os.path.expandvars(value) if type(value) is str else value for value in tool]
    args = env_tool + [filepath, '--output=json'] + exec_args
    with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env) as process:
        stdout, stderr = process.communicate(timeout=kwargs.get('timeout', 240))

        stdout_decoded = stdout.decode('utf-8') if stdout else None
        stderr_decoded = stderr.decode('utf-8') if stderr and process.returncode != 0 else None

        if stdout_decoded:
            try:
                return json.loads(stdout_decoded), stderr_decoded
            except ValueError:
                logger.exception('invalid benchmark result: \'%s\'', stdout_decoded)

        return None, stderr_decoded


def valgrind_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind', '--error-exitcode=1'], exec_args, **kwargs)


def callgrind_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind', '--error-exitcode=1', '--tool=callgrind'], exec_args, **kwargs)


def drmemory_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['${DR_MEMORY}', '-exit_code_if_errors', '1', '--'], exec_args, **kwargs)


def qemu_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['qemu-x86_64'], exec_args, **kwargs)


def strace_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['strace', '-o', 'test'], exec_args, **kwargs)


def ltrace_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['ltrace', '-o', 'test'], exec_args, **kwargs)


valgrind_kwargs = {'build_system_func': valgrind_build_system_executor, 'exec_func': valgrind_executor}
callgrind_kwargs = {'build_system_func': valgrind_build_system_executor, 'exec_func': callgrind_executor}
drmemory_kwargs = {'build_system_func': drmemory_build_system_executor, 'exec_func': drmemory_executor}
qemu_kwargs = {'build_system_func': qemu_build_system_executor, 'exec_func': qemu_executor}
strace_kwargs = {'build_system_func': strace_build_system_executor, 'exec_func': strace_executor}
ltrace_kwargs = {'build_system_func': ltrace_build_system_executor, 'exec_func': ltrace_executor}

harness.add_runtime('valgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, **valgrind_kwargs)
harness.add_runtime('callgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, **callgrind_kwargs)
harness.add_runtime('drmemory-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **drmemory_kwargs)
harness.add_runtime('qemu-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **qemu_kwargs)
harness.add_runtime('boehmgc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('diehard-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('rpmalloc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('jemalloc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('hoard-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('freeguard-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('libdislocator-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('tcmalloc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('dlmalloc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('tlsf-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('strace-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **strace_kwargs)
harness.add_runtime('ltrace-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **ltrace_kwargs)
