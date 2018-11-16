import glob
import os
import subprocess

from functools import partial

PGO_SUFFIX = 'test_pgo'
PGO_PROFRAW_FILE_SUFFIX = '.profraw'
PGO_PROFRAW_MERGED_FILE_SUFFIX = PGO_PROFRAW_FILE_SUFFIX + '.merged'


def pgo_build_system_executor(make_env):
    return build_system_executor(make_env, cc_version='--version', as_version='--version')


def pgo_cleam(workdir, **kwargs):
    assert os.path.isdir(workdir)

    # clean directory with pgo suffix
    logger.info('clean pgo files')
    with subprocess.Popen(['make', 'clean', 'TEST_SUFFIX=' + PGO_SUFFIX], cwd=workdir, stdout=subprocess.DEVNULL) as process:
        process.communicate()

        if process.returncode != 0:
            logger.error('"make clean" exited with non zero return code!')
            return False

    # remove profiling informations
    profraw_files = glob.glob(os.path.join(workdir, '*' + PGO_PROFRAW_FILE_SUFFIX))
    profraw_merged_files = glob.glob(os.path.join(workdir, '*' + PGO_PROFRAW_MERGED_FILE_SUFFIX))
    for f in profraw_files + profraw_merged_files:
        os.remove(f)

    # clean normal test files
    return default_clean(workdir, **kwargs)


def pgo_make(workdir, make_env, **kwargs):
    pgo_make_env = make_env.copy()
    pgo_make_env['CFLAGS'] = pgo_make_env['CFLAGS_PGO']
    pgo_make_env['LDFLAGS'] = pgo_make_env['LDFLAGS_PGO']
    del pgo_make_env['CFLAGS_PGO']
    del pgo_make_env['LDFLAGS_PGO']

    return default_make(workdir, pgo_make_env, **kwargs)


def pgo_exec(filepath, workdir, exec_args, **kwargs):
    assert os.path.isfile(filepath)
    assert os.path.isdir(workdir)

    filepath_pgo = filepath + '_pgo'
    if not os.path.exists(filepath_pgo):
        profraw_file = os.path.abspath(os.path.join(workdir, filepath + PGO_PROFRAW_FILE_SUFFIX))
        profraw_file_merged = os.path.abspath(os.path.join(workdir, filepath + PGO_PROFRAW_MERGED_FILE_SUFFIX))

        args = [filepath, '--output=json'] + exec_args
        env = os.environ.copy()
        for key, val in run_env.items():
            env[key] = val

        env['LLVM_PROFILE_FILE'] = profraw_file

        # execute with profiler
        if os.path.isfile(profraw_file):
            os.remove(profraw_file)
        with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env) as process:
            stdout, stderr = process.communicate(timeout=kwargs.get('timeout', 240))

            if not os.path.isfile(profraw_file):
                return None, stderr.decode('utf-8') if stderr else None

        # merge profiling files (required)
        with subprocess.Popen(['llvm-profdata', 'merge', '-output='+profraw_file_merged, profraw_file], cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env) as process:
            stdout, stderr = process.communicate(timeout=10)
            if process.returncode is not 0 or not os.path.isfile(profraw_file_merged):
                logger.error('cannot merge profdata')
                return None, stderr.decode('utf-8') if stderr else None

        # remove library to cause recompilation
        # TODO: better solution?
        os.remove(os.path.join(workdir, '../C-Hayai/build/src/libchayai.a'))

        # make optimized binary
        make_env = kwargs['make_env_copy'].copy()
        logger.info(make_env)
        make_env['TEST_SUFFIX'] = PGO_SUFFIX
        make_env['CFLAGS'] = make_env.get('CFLAGS', '') + ' -fprofile-instr-use=' + profraw_file_merged
        make_env['LDFLAGS'] = make_env.get('LDFLAGS', '') + ' -fprofile-instr-use=' + profraw_file_merged
        del make_env['CFLAGS_PGO']
        del make_env['LDFLAGS_PGO']
        make_kwargs = kwargs.copy()
        make_kwargs['make_target'] = os.path.basename(filepath_pgo)
        make_kwargs['ignore_errors'] = False
        if not default_make(workdir, make_env, **make_kwargs):
            logger.error('recompilation with PGO failed')
            return None, None

    # execute optimized binary
    return default_executor(filepath_pgo, workdir, exec_args, **kwargs)


clang_pgo_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version'), 'clean_func': pgo_cleam, 'make_func': pgo_make, 'exec_func': pgo_exec}

harness.add_runtime('clang-pgo-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-O3", "CFLAGS_PGO": "-O3 -flto -fprofile-instr-generate", "LDFLAGS_PGO": "-fprofile-instr-generate"}, **clang_pgo_kwargs)