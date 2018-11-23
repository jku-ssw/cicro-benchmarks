import glob
import os
import subprocess

from functools import partial


def pgo_clean(workdir, pgo_suffix, profile_files, **kwargs):
    assert os.path.isdir(workdir)
    assert pgo_suffix

    # clean directory with pgo suffix
    logger.info('clean pgo files')
    with subprocess.Popen(['make', 'clean', 'TEST_SUFFIX=' + pgo_suffix], cwd=workdir, stdout=subprocess.DEVNULL) as process:
        process.communicate()

        if process.returncode != 0:
            logger.error('"make clean" exited with non zero return code!')
            return False

    # remove profiling informations
    for f_suffix in profile_files:
        for f in glob.glob(os.path.join(workdir, '*' + f_suffix)):
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


def pgo_clean_lib(workdir):
    # TODO: better solution?
    libchayai_path = os.path.join(workdir, '../C-Hayai/build/src/libchayai.a')
    if os.path.isfile(libchayai_path):
        os.remove(libchayai_path)


def pgo_recompile_profiling(workdir, make_target, pgo_suffix, cflags, ldflags, **kwargs):
    # make optimized binary
    make_env = kwargs['make_env_copy'].copy()
    make_env['TEST_SUFFIX'] = pgo_suffix
    make_env['CFLAGS'] = make_env.get('CFLAGS', '') + cflags
    make_env['LDFLAGS'] = make_env.get('LDFLAGS', '') + ldflags
    del make_env['CFLAGS_PGO']
    del make_env['LDFLAGS_PGO']
    make_kwargs = kwargs.copy()
    make_kwargs['make_target'] = make_target
    make_kwargs['ignore_errors'] = False
    if not default_make(workdir, make_env, **make_kwargs):
        logger.error('recompilation with PGO failed')
        return None, None


CLANG_PGO_SUFFIX = 'test_pgo'
CLANG_PGO_PROFRAW_FILE_SUFFIX = '.profraw'
CLANG_PGO_PROFRAW_MERGED_FILE_SUFFIX = CLANG_PGO_PROFRAW_FILE_SUFFIX + '.merged'


def clang_pgo_exec(filepath, workdir, exec_args, **kwargs):
    assert os.path.isfile(filepath)
    assert os.path.isdir(workdir)

    filepath_pgo = filepath + '_pgo'
    if not os.path.exists(filepath_pgo):
        profraw_file = os.path.abspath(os.path.join(workdir, filepath + CLANG_PGO_PROFRAW_FILE_SUFFIX))
        profraw_file_merged = os.path.abspath(os.path.join(workdir, filepath + CLANG_PGO_PROFRAW_MERGED_FILE_SUFFIX))

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
        with subprocess.Popen([os.path.expandvars(env['LLVM_PROFDATA']), 'merge', '-output=' + profraw_file_merged, profraw_file], cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env) as process:
            stdout, stderr = process.communicate(timeout=10)
            if process.returncode is not 0 or not os.path.isfile(profraw_file_merged):
                logger.error('cannot merge profdata')
                return None, stderr.decode('utf-8') if stderr else None

        # remove library to cause recompilation
        pgo_clean_lib(workdir)

        # make optimized binary
        cflags = ' -fprofile-instr-use=' + profraw_file_merged
        ldflags = ' -fprofile-instr-use=' + profraw_file_merged
        res = pgo_recompile_profiling(workdir, os.path.basename(filepath_pgo), CLANG_PGO_SUFFIX, cflags, ldflags, **kwargs)
        if res == (None, None):
            return res

    # execute optimized binary
    return default_executor(filepath_pgo, workdir, exec_args, **kwargs)


GCC_PGO_SUFFIX = 'test_pgo'
GCC_PGO_FILE_SUFFIX = '.gcda'


def gcc_pgo_exec(filepath, workdir, exec_args, **kwargs):
    assert os.path.isfile(filepath)
    assert os.path.isdir(workdir)

    filepath_pgo = filepath + '_pgo'
    if not os.path.exists(filepath_pgo):
        pgo_file = os.path.abspath(os.path.join(workdir, filepath + GCC_PGO_FILE_SUFFIX))

        res = default_executor(filepath, workdir, exec_args, **kwargs)
        if res[0] is None:
            return res

        # remove library to cause recompilation
        pgo_clean_lib(workdir)

        # make optimized binary
        cflags = ' -fprofile-use=' + pgo_file
        ldflags = ' -fprofile-use=' + pgo_file
        res = pgo_recompile_profiling(workdir, os.path.basename(filepath_pgo), GCC_PGO_SUFFIX, cflags, ldflags, **kwargs)
        if res == (None, None):
            return res

    # execute optimized binary
    return default_executor(filepath_pgo, workdir, exec_args, **kwargs)


clang_pgo_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version'),
                    'clean_func': partial(pgo_clean, pgo_suffix=CLANG_PGO_SUFFIX, profile_files=[CLANG_PGO_PROFRAW_FILE_SUFFIX, CLANG_PGO_PROFRAW_MERGED_FILE_SUFFIX]),
                    'make_func': pgo_make,
                    'exec_func': clang_pgo_exec
                    }

gcc_pgo_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version'),
                  'clean_func': partial(pgo_clean, pgo_suffix=GCC_PGO_SUFFIX, profile_files=[GCC_PGO_FILE_SUFFIX]),
                  'make_func': pgo_make,
                  'exec_func': gcc_pgo_exec
                  }

harness.add_runtime('clang-pgo-O3', {"CC": "${CLANG}", "AS": "${CLANG}",
                                     "CFLAGS": "-O3",
                                     "CFLAGS_PGO": "-O3 -flto -fprofile-instr-generate",
                                     "LDFLAGS_PGO": "-fprofile-instr-generate -fuse-ld=lld"}, **clang_pgo_kwargs)  # NOQA: E501
harness.add_runtime('gcc-pgo-O3', {"CC": "${GCC}", "AS": "as",
                                   "CFLAGS": "-std=gnu99 -O3",
                                   "CFLAGS_PGO": "-O3 -fprofile-generate",
                                   "LDFLAGS_PGO": "-fprofile-generate"}, **gcc_pgo_kwargs)