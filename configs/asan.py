harness.add_runtime('asan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-fno-common -Wno-everything -O3 -fsanitize=address"})
harness.add_runtime('msan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=memory"})
harness.add_runtime('ubsan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=undefined"})
harness.add_runtime('tsan-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3 -fsanitize=thread"})
