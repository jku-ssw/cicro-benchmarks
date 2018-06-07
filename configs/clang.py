# Clang
harness.add_runtime('clang-O0', {"CC": "clang", "AS": "clang", "CFLAGS": "-Wno-everything -O0"})
harness.add_runtime('clang-O1', {"CC": "clang", "AS": "clang", "CFLAGS": "-Wno-everything -O1"})
harness.add_runtime('clang-O2', {"CC": "clang", "AS": "clang", "CFLAGS": "-Wno-everything -O2"})
harness.add_runtime('clang-O3', {"CC": "clang", "AS": "clang", "CFLAGS": "-Wno-everything -O3"})
harness.add_runtime('clang-Os', {"CC": "clang", "AS": "clang", "CFLAGS": "-Wno-everything -Os"})
