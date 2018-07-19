import os
import subprocess


def lli_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen([os.path.expandvars('${LLI}'), '--version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['LLI_version'] = stdout_decoded

    return result


def lli_executor(filepath, workdir, exec_args, **kwargs):
    return wllvm_executor(filepath, workdir, '${LLI}', exec_args, **kwargs)


lli_kwargs = {'build_system_func': lli_build_system_executor,
              'make_func': wllvm_make,
              'exec_func': lli_executor}

harness.add_runtime('lli-O3', {"CC": "${WLLVM_DIR}/wllvm", "AS": "${WLLVM_DIR}/wllvm", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, **lli_kwargs)
