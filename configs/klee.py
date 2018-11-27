import os
import subprocess


def klee_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen([os.path.expandvars('${KLEE}'), '-version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['KLEE_version'] = stdout_decoded

    return result


def klee_executor(filepath, workdir, exec_args, **kwargs):
    return wllvm_executor(filepath, workdir, '${KLEE} --optimize --libc=uclibc --posix-runtime', exec_args, **kwargs)


klee_kwargs = {'build_system_func': klee_build_system_executor, 'make_func': wllvm_make, 'exec_func': klee_executor}

harness.add_runtime('klee-O3', {"CC": "${WLLVM_DIR}/wllvm", "AS": "${WLLVM_DIR}/wllvm", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, **klee_kwargs)
