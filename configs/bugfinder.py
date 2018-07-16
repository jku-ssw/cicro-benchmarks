# those tests are intended to find bugs in the benchmarks itself

gcc_cflags = [
    "-std=gnu99",  # otherwise our CI does not finish
    "-Wno-everything",  # no warnings
    "-O2",  # some optimizations
    "-g",  # debug symbols
    "-fsanitize=address",  # enable address sanitizer
    "-D_FORTIFY_SOURCE=2",  # detect some buffer overflows
    "-Wl,-z,relro,-z,now",  # full RELRO
    "-fstack-protector-all"
]
harness.add_runtime('bugfinder-asan-gcc-O2', {"CC": "${GCC}", "AS": "as", "CFLAGS": " ".join(gcc_cflags)})

clang_cflags = [
    "-Wno-everything",  # no warnings
    "-O2",  # some optimizations
    "-g",  # debug symbols
    "-fsanitize=address",  # enable address sanitizer
    "-D_FORTIFY_SOURCE=2",  # detect some buffer overflows
    "-Wl,-z,relro,-z,now",  # full RELRO
    "-fstack-protector-all"
]
harness.add_runtime('bugfinder-asan-clang-O2', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": " ".join(clang_cflags)})

harness.add_runtime('bugfinder-msan-clang-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -g -fsanitize=memory"})
harness.add_runtime('bugfinder-ubsan-clang-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -g -fsanitize=undefined"})
harness.add_runtime('bugfinder-tsan-clang-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -g -fsanitize=thread"})
