from functools import partial

clang_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('asan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-fno-common -Wno-everything -O3 -fsanitize=address"}, **clang_kwargs)
harness.add_runtime('msan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=memory"}, **clang_kwargs)
harness.add_runtime('ubsan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=undefined"}, **clang_kwargs)
harness.add_runtime('tsan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=thread"}, **clang_kwargs)
