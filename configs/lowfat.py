from functools import partial

clang_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}
harness.add_runtime('lowfat-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -fsanitize=lowfat -O3"}, **clang_kwargs)
