all: gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json perf.json sulong-warmup.json

.DELETE_ON_ERROR:

RUNS=2

sulong-warmup.json:
	../tools/bench.py -y --filter-runtime="(sulong-warmup-0|sulong-warmup-2)" sulong-warmup.json --filter-harness="(?!(faragon-libsrt|armon-libart))" --exec-args="--iterations=1" --runs=1

gcc.json:
	../tools/bench.py -y --filter-runtime="gcc" gcc.json --exec-args="--iterations=10" --runs=${RUNS}

clang.json:
	../tools/bench.py -y --filter-runtime="clang" clang.json --exec-args="--iterations=10" --runs=${RUNS}

compcert.json:
	../tools/bench.py -y --filter-runtime="clang" compcert.json --exec-args="--iterations=10" --runs=${RUNS}  --ignore-errors

sanitizers.json:
	../tools/bench.py -y --filter-runtime="(asan-O3|msan-O3|tsan-O3|ubsan-O3)" sanitizers.json --exec-args="--iterations=10" --runs=${RUNS}  --ignore-errors

tcc.json:
	../tools/bench.py -y --filter-runtime="tcc" tcc.json --exec-args="--iterations=10" --runs=${RUNS} --ignore-errors

valgrind.json:
	../tools/bench.py -y --filter-runtime="valgrind-O3" valgrind.json --exec-args="--iterations=10" --runs=${RUNS}

callgrind.json:
	../tools/bench.py -y --filter-runtime="callgrind-O3" callgrind.json --exec-args="--iterations=10" --runs=${RUNS}

perf.json:
	../tools/bench.py -y --filter-runtime="(gcc-O3|clang-O3)" perf.json --exec-args="--papi=PAPI_TOT_CYC,PAPI_TOT_INS,PAPI_BR_INS" --runs=${RUNS}
