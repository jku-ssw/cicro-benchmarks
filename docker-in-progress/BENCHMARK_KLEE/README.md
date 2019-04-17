# KLEE

## execution errors

some of the functions used are not found by klee - few of them are furthermore located in the benchmark harness

```
 [ ERROR    ]  cannot add benchmark run without data (bench_results.py:151)
[2018-11-27 11:22:06,381] [ INFO     ]  benchmark: "RoaringBitmap-CRoaring:klee-O3" (bench.py:372)
[2018-11-27 11:22:07,077] [ WARNING  ]  benchmark harness had some output on stderr:
KLEE: NOTE: Using POSIX model: /klee_build_dir/Debug+Asserts/lib/libkleeRuntimePOSIX.bca
KLEE: NOTE: Using klee-uclibc : /klee_build_dir/Debug+Asserts/lib/klee-uclibc.bca
KLEE: output directory is "/tmp/tmpd4ikigzf/klee-out-0"
KLEE: Using STP solver backend
LLVM ERROR: Code generator does not support intrinsic function 'llvm.x86.sse2.storeu.dq'!
 (bench.py:384)
[2018-11-27 11:22:07,077] [ ERROR    ]  cannot add benchmark run without data (bench_results.py:151)
[2018-11-27 11:22:07,711] [ INFO     ]  benchmark: "Themaister-libfmsynth:klee-O3" (bench.py:372)
[2018-11-27 11:22:08,671] [ WARNING  ]  benchmark harness had some output on stderr:
KLEE: NOTE: Using POSIX model: /klee_build_dir/Debug+Asserts/lib/libkleeRuntimePOSIX.bca
KLEE: NOTE: Using klee-uclibc : /klee_build_dir/Debug+Asserts/lib/klee-uclibc.bca
KLEE: output directory is "/tmp/tmpsfecr2p5/klee-out-0"
KLEE: Using STP solver backend
KLEE: WARNING: undefined reference to function: argp_parse
KLEE: WARNING: undefined reference to function: clock_getres
KLEE: WARNING: undefined reference to function: exp2f
KLEE: WARNING: undefined reference to function: expf
KLEE: WARNING: undefined reference to function: floorf
KLEE: WARNING: undefined reference to function: logf
KLEE: WARNING: undefined reference to function: powf
KLEE: WARNING: undefined reference to function: sqrt
KLEE: WARNING ONCE: calling external: syscall(16, 0, 21505, 94055963043616) at /klee/runtime/POSIX/fd.c:980
KLEE: WARNING ONCE: Alignment of memory from call "malloc" is not modelled. Using alignment of 8.
KLEE: WARNING ONCE: calling external: argp_parse(94055962685184, 2, 94055958720960, 0, 0, 94055958664640) at /klee/runtime/POSIX/fd_init.c:160
KLEE: ERROR: /klee/runtime/POSIX/fd_init.c:160: failed external call: argp_parse
KLEE: NOTE: now ignoring this error at this location

KLEE: done: total instructions = 7756
KLEE: done: completed paths = 1
KLEE: done: generated tests = 1
```
