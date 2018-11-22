from functools import partial

clang_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('clang-O0', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O0"}, **clang_kwargs)
harness.add_runtime('clang-O1', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O1"}, **clang_kwargs)
harness.add_runtime('clang-O2', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O2"}, **clang_kwargs)
harness.add_runtime('clang-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, **clang_kwargs)
harness.add_runtime('clang-lld-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "LDFLAGS": "-fuse-ld=lld"}, **clang_kwargs)
harness.add_runtime('clang-gold-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "LDFLAGS": "-fuse-ld=gold"}, **clang_kwargs)
harness.add_runtime('clang-march-native-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -march=native"}, **clang_kwargs)
harness.add_runtime('clang-Ofast', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -Ofast"}, **clang_kwargs)
harness.add_runtime('clang-Os', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -Os"}, **clang_kwargs)
harness.add_runtime('clang-fno-strict-aliasing-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fno-strict-aliasing"}, **clang_kwargs)
harness.add_runtime('clang-fno-strict-overflow-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fno-strict-overflow"}, **clang_kwargs)
harness.add_runtime('clang-fno-delete-null-pointer-checks-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fno-delete-null-pointer-checks"}, **clang_kwargs)
harness.add_runtime('clang-mno-retpoline-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -mno-retpoline"}, **clang_kwargs)
harness.add_runtime('clang-safe-stack-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=safe-stack"}, **clang_kwargs)
harness.add_runtime('clang-lld-cfi-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=cfi -flto -fvisibility=hidden", "LDFLAGS": "-fuse-ld=lld"}, **clang_kwargs)
harness.add_runtime('polly-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -mllvm -polly"}, **clang_kwargs)
