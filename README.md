# C Benchmarking Suite

[![Build Status](https://travis-ci.org/jku-ssw/benchmarks.svg?branch=master)](https://travis-ci.org/jku-ssw/benchmarks)

This is a benchmarking suite for C compilers, optimizers and runtime environments. It allows us to measure how changes
affect runtime performance in a general manner and to not be dependent on a few single benchmarks. Compared to other
benchmarking suites our approach is to use a high number of "real world" code samples to get known of the general runtime
impact.

## Installation

It should be as simple as downloading the repository and starting a benchmark. This causes everything to be compiled
and then executed in the defined runtime enviroment.

### Dependencies

We tried to keep the number of dependencies low. In fact [PAPI](http://icl.utk.edu/papi/) should be the only dependency
which is not installed by default on a common linux distribution. This dependency is only required if there is interest
in reading the hardware counters aside from the actual runtime.

Shared libraries used:

* -lm
* -lpthread
* -lpapi (optional)


## HowTo Use

In the ```./tools/``` directory there are a few scripts which manage most of the benchmarking work.

### Run Benchmark

```bench.py``` manages all steps necessary to build the benchmark suite, to run it, and to write the results back. There
is a basic set of runtime enviroments specified in the ```./configs``` directory (gcc, clang, tcc,...) which shows how
runtime environments are specified.

**Example**: run gcc with -O3 and write the results into ```my_results.json```:

```
./tools/bench.py my_results.json --filter-runtime=gcc-O3
```

**Example**: run all clang runtimes and execute all harnesses where the name starts with ```a```. Furthermore instrument some hardware counters:

```
./tools/bench.py example_results.json --filter-harness="^a.*" --filter-runtime="clang-.*" --exec-args="--papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS"
```

Please note it is important to configure the system in a way to have as least impact on the benchmarks as possible.
This means for example disabling networking and all applications which are not mandatory for the run. Beside of that
the script warns when the ```scaling_governor``` of the cpu is set to the wrong state. The benchmark executor warns when
known performance settings are not set correctly.

### Analyze Benchmark Results

```
./tools/stats.py my_results.json
```

This tool prints some basic informations which could be usefull to find missing tests for example.

### Plot Benchmark Results

```
./tools/plot.R --benchfile=example_results.json --base-runtime=clang-O0
```

Please note, currently plotting requires a baseline, and thus at least two runs are required to actual plot something!

## Some notes about hardware counters

When you are interested about measuring hardware counters, you first need to know which counters are available:

```bash
papi_avail
```

It is recommended to check, if the selected hardware counters can be combinded together. If they cannot be combinded,
you are required to do multiple runs with different counters, to get all measurements:

```bash
papi_command_line PAPI_LD_INS PAPI_SR_INS PAPI_BR_INS PAPI_TOT_INS PAPI_TOT_CYC
```

To enable the counters, you need to pass a list of them to the benchmark harness. It should be pointed out that counter
results are only visible in the json output, not in the default console output. You can run benchmarks without the
harness runner to try out the settings:

```bash
./my_benchmark_test --output=json --papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS
```
