# ack

## Execution Error

```
[2018-11-23 09:59:21,347] [ INFO     ]  Following runtimes will be executed: (bench.py:615)
[2018-11-23 09:59:21,347] [ INFO     ]   * ack-O3 (bench.py:617)
Do you want to continue with the benchmarks? [Y/n] [2018-11-23 09:59:21,370] [ WARNING  ]  please set all scaling_governor to "performance" (using "sudo ./tools/ondemand.sh start") (bench.py:100)

[2018-11-23 09:59:23,244] [ INFO     ]  start with the execution of the runtime enviroment "ack-O3" (bench.py:278)
[2018-11-23 09:59:23,361] [ INFO     ]  execute clean step for "ack-O3" (bench.py:297)
[2018-11-23 09:59:23,361] [ INFO     ]  clean benchmark directory (bench.py:129)
Makefile:489: *** Cannot proceed: makefile fragments not detected! Run configure first.  Stop.
rmdir: failed to remove 'src_c': Directory not empty
rmdir: failed to remove 'python_out': No such file or directory
rmdir: failed to remove 'jsx_out': No such file or directory
[2018-11-23 09:59:23,747] [ INFO     ]  execute compilation step for "ack-O3" (bench.py:306)
mkdir -p ../C-Hayai/build
(cd ../C-Hayai/build && cmake ../ -DCMAKE_C_COMPILER='/usr/local/bin/ack' -DCMAKE_C_FLAGS='-mlinux386 -ansi -O3' -DUSE_PAPI=ON)  #-DCMAKE_STATIC_LINKER_FLAGS=''
-- The C compiler identification is unknown
-- Check for working C compiler: /usr/local/bin/ack
-- Check for working C compiler: /usr/local/bin/ack -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Looking for clock_gettime in rt
-- Looking for clock_gettime in rt - not found
-- Configuring done
-- Generating done
-- Build files have been written to: /benchmarks/C-Hayai/build
make -C ../C-Hayai/build -j4 AS='/usr/local/bin/ack' CC='/usr/local/bin/ack' CFLAGS='-mlinux386 -ansi -O3' LDFLAGS='' chayai
make[1]: warning: -jN forced in submake: disabling jobserver mode.
make[1]: Entering directory '/benchmarks/C-Hayai/build'
make[2]: Entering directory '/benchmarks/C-Hayai/build'
make[3]: Entering directory '/benchmarks/C-Hayai/build'
make[4]: Entering directory '/benchmarks/C-Hayai/build'
Scanning dependencies of target chayai
make[4]: Leaving directory '/benchmarks/C-Hayai/build'
make[4]: Entering directory '/benchmarks/C-Hayai/build'
[ 22%] Building C object src/CMakeFiles/chayai.dir/chayai_clock.c.o
[ 22%] Building C object src/CMakeFiles/chayai.dir/chayai_benchmarker.c.o
[ 44%] Building C object src/CMakeFiles/chayai.dir/chayai_console.c.o
[ 44%] Building C object src/CMakeFiles/chayai.dir/chayai_console_outputter.c.o
"/benchmarks/C-Hayai/src/chayai_clock.h", line 93: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 101: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 105: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 105: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 109: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 109: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 113: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.c", line 5: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.c", line 5: (warning) 'chayai_clock_resolution_measured' old-fashioned function definition
"/benchmarks/C-Hayai/src/chayai_clock.c", line 6: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.c", line 6: (warning) constant too large for target machine
"/benchmarks/C-Hayai/src/chayai_clock.c", line 10: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.c", line 11: ; missing before int
"/benchmarks/C-Hayai/src/chayai_clock.c", line 11: < deleted
"/benchmarks/C-Hayai/src/chayai_clock.c", line 14: p2 undefined
"/benchmarks/C-Hayai/src/chayai_clock.c", line 15: cur_time undefined
"/benchmarks/C-Hayai/src/chayai_clock.c", line 15: p2 undefined
"/benchmarks/C-Hayai/src/chayai_clock.c", line 17: min_time undefined
"/benchmarks/C-Hayai/src/chayai_clock.c", line 22: return deleted
"/benchmarks/C-Hayai/src/chayai_clock.c", line 23: } deleted
"/benchmarks/C-Hayai/src/chayai_clock.c", line 211: (warning) 'chayai_clock_now' old-fashioned function definition
"/benchmarks/C-Hayai/src/chayai_clock.c", line 217: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.c", line 223: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.c", line 223: (warning) 'chayai_clock_resolution' old-fashioned function definition
"/benchmarks/C-Hayai/src/chayai_clock.c", line 227: (warning) 'chayai_clock_type_str' old-fashioned function definition
src/CMakeFiles/chayai.dir/build.make:86: recipe for target 'src/CMakeFiles/chayai.dir/chayai_clock.c.o' failed
make[4]: *** [src/CMakeFiles/chayai.dir/chayai_clock.c.o] Error 1
make[4]: *** Waiting for unfinished jobs....
"/benchmarks/C-Hayai/src/chayai_benchmarker.c", line 17: cannot open include file "papi.h"
Pre-processor failed
src/CMakeFiles/chayai.dir/build.make:62: recipe for target 'src/CMakeFiles/chayai.dir/chayai_benchmarker.c.o' failed
make[4]: *** [src/CMakeFiles/chayai.dir/chayai_benchmarker.c.o] Error 1
"/benchmarks/C-Hayai/src/chayai_benchmark_result.h", line 8: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_benchmark_result.h", line 11: repeated size specifier
"/benchmarks/C-Hayai/src/chayai_benchmark_result.h", line 20: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_benchmark_result.h", line 21: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_benchmark_result.h", line 22: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 93: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 101: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 105: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 105: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 109: int64_t is not a type identifier
"/benchmarks/C-Hayai/src/chayai_clock.h", line 109: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_clock.h", line 113: (warning) old-fashioned function declaration
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 39: making function body for non-function
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 43: (warning) chayai_console_outputter_deviation is a function; cannot be formal
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 43: chayai_console_outputter_deviation not in parameter list
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 45: static formal illegal
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 45: inline not in parameter list
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 45: ; missing before void
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 49: (warning) chayai_console_outputter_deviation_inverse is a function; cannot be formal
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 49: chayai_console_outputter_deviation_inverse not in parameter list
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 53: (warning) chayai_console_outputter_init is a function; cannot be formal
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 53: chayai_console_outputter_init not in parameter list
"/benchmarks/C-Hayai/src/chayai_console_outputter.c", line 53: ; missing before {
/usr/local/bin/ack: /usr/local/lib/ack/em_cemcom.ansi died with signal 11
src/CMakeFiles/chayai.dir/build.make:134: recipe for target 'src/CMakeFiles/chayai.dir/chayai_console_outputter.c.o' failed
make[4]: Leaving directory '/benchmarks/C-Hayai/build'
make[4]: *** [src/CMakeFiles/chayai.dir/chayai_console_outputter.c.o] Error 1
CMakeFiles/Makefile2:85: recipe for target 'src/CMakeFiles/chayai.dir/all' failed
make[3]: Leaving directory '/benchmarks/C-Hayai/build'
make[3]: *** [src/CMakeFiles/chayai.dir/all] Error 2
make[2]: *** [src/CMakeFiles/chayai.dir/rule] Error 2
CMakeFiles/Makefile2:97: recipe for target 'src/CMakeFiles/chayai.dir/rule' failed
make[2]: Leaving directory '/benchmarks/C-Hayai/build'
make[1]: *** [chayai] Error 2
Makefile:118: recipe for target 'chayai' failed
make[1]: Leaving directory '/benchmarks/C-Hayai/build'
make: *** [../C-Hayai/build/src/libchayai.a] Error 2
Makefile:67: recipe for target '../C-Hayai/build/src/libchayai.a' failed
[2018-11-23 09:59:26,046] [ ERROR    ]  "make" of benchmark harness exited with non zero return code! (bench.py:152)
[2018-11-23 09:59:26,046] [ ERROR    ]  compilation step for "ack-O3" failed (bench.py:308)
[2018-11-23 09:59:26,047] [ INFO     ]  write results into benchmark file (bench.py:643)
[2018-11-23 09:59:26,285] [ ERROR    ]  runtime "ack-O3" did not finish successful (bench.py:242)
```
