from functools import partial

clang_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('clang-O0', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O0"}, **clang_kwargs)
harness.add_runtime('clang-O1', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O1"}, **clang_kwargs)
harness.add_runtime('clang-O2', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O2"}, **clang_kwargs)
harness.add_runtime('clang-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **clang_kwargs)
harness.add_runtime('clang-Os', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -Os"}, **clang_kwargs)
