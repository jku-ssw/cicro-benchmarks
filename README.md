# C Benchmarking Suite

this is a benchmarking Suite for C compilers, optimiziers and runtime enviroments to get known how changes affect runtime
performance in general. Compared to other benchmarking suites our approach is to use a high number of "real world" code
samples to get known of the general runtime impact.

## HowTo Use

in the ```./tools/``` directory there are a few scripts which manage most of the benchmarking work.

### run benchmark

```bench.py``` manages all steps necessary to build the benchmark suite, run it, and write the results back. There is a
basic set of runtime enviroments specified (gcc, clang and tcc) which shows how new runtime enviroments are specified.

Run gcc with O3 and write the results in ```my_results.json```:

```
./tools/bench.py ./benchmarks/ my_results.json --filter-runtime=gcc-O3
```

### analyze benchmark results

```
./tools/stats.py my_results.json
```

This tool prints some basic informations which could be usefull to find missing tests for example.

### plot benchmark results

```
./tools/plot.py my_results.json gcc-O3
```

Please note, currently plotting requires a baseline, and thus at least two runs are required to actual plot something!
