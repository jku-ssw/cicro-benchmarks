import os
import subprocess

from functools import partial


def graalvm_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    with subprocess.Popen([os.path.expandvars('$GRAALVM_DIR/lli'), '--version'], stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['GRAALVM_version'] = stdout_decoded

    return result


def sulong_build_system_executor(make_env):
    result = build_system_executor(make_env, cc_version='--version', as_version='--version')

    args = ['mx', '-p', os.path.expandvars('${SULONG_DIR}'), 'lli', '--version']
    with subprocess.Popen(args, stdout=subprocess.PIPE) as p:
        stdout, _ = p.communicate(timeout=1)

        stdout_decoded = stdout.decode('utf-8').rstrip() if stdout else None
        if p.returncode == 0 and stdout_decoded:
            result['SULONG_version'] = stdout_decoded

    return result


def sulong_graalvm_executor(filepath, workdir, exec_args, warmup, **kwargs):
    return wllvm_executor(filepath, workdir, '$GRAALVM_DIR/lli', exec_args + ["--warmup={}".format(warmup)], **kwargs)


def sulong_latest_executor(filepath, workdir, exec_args, warmup, **kwargs):
    kwargs['timeout'] = 10000
    sulong_cmd = ' '.join(['mx', '--dynamicimport', '/compiler' '--jdk', 'jvmci' '-p', os.path.expandvars('${SULONG_DIR}'), 'lli'])
    return wllvm_executor(filepath, workdir, sulong_cmd, exec_args + ["--warmup={}".format(warmup)], **kwargs)


sulong_env = {
    "CC": "${WLLVM_DIR}/wllvm",
    "AS": "${WLLVM_DIR}/wllvm",
    "CFLAGS": "-Wno-everything -O1",
    "PAPI": 0
}

graalvm_kwargs = {'build_system_func': graalvm_build_system_executor}
sulong_kwargs = {'build_system_func': sulong_build_system_executor}

harness.add_runtime('sulong-graalvm', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_graalvm_executor, warmup=100), **graalvm_kwargs)

harness.add_runtime('sulong-warmup-0', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=0), **sulong_kwargs)
harness.add_runtime('sulong-warmup-1', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=1), **sulong_kwargs)
harness.add_runtime('sulong-warmup-2', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=2), **sulong_kwargs)
harness.add_runtime('sulong-warmup-10', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=10), **sulong_kwargs)
harness.add_runtime('sulong-warmup-100', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=100), **sulong_kwargs)
harness.add_runtime('sulong-warmup-1000', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=1000), **sulong_kwargs)
harness.add_runtime('sulong-warmup-10000', sulong_env, make_func=wllvm_make, exec_func=partial(sulong_latest_executor, warmup=10000), **sulong_kwargs)
