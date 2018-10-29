from functools import partial

gcc_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('gcc-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0"}, **gcc_kwargs)
harness.add_runtime('gcc-O1', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O1"}, **gcc_kwargs)
harness.add_runtime('gcc-O2', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O2"}, **gcc_kwargs)
harness.add_runtime('gcc-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3"}, **gcc_kwargs)
harness.add_runtime('gcc-march-native-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -march=native"}, **gcc_kwargs)
harness.add_runtime('gcc-Ofast', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -Ofast"}, **gcc_kwargs)
harness.add_runtime('gcc-Os', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -Os"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-strict-aliasing-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-strict-aliasing"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-strict-overflow-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-strict-overflow"}, **gcc_kwargs)
harness.add_runtime('gcc-fno-delete-null-pointer-checks-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O3 -fno-delete-null-pointer-checks"}, **gcc_kwargs)
harness.add_runtime('gcc-gcov-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0 -fprofile-arcs -ftest-coverage"}, **gcc_kwargs)
harness.add_runtime('gcc-gprof-O0', {"CC": "${GCC}", "AS": "as", "CFLAGS": "-std=gnu99 -O0 -pg"}, **gcc_kwargs)
harness.add_runtime('gcc-mpx-O3', {"CC": "${GCC}", "AS": "as", "CFLAGS": " -std=gnu99 -O3 -mmpx -fcheck-pointer-bounds -lmpxwrappers"}, **gcc_kwargs)
