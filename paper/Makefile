.PHONY: all clean

all: gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json sulong-warmup.json no-papi.json

clean:
	rm -f *.json Rplots.pdf

.DELETE_ON_ERROR:  # TODO: script return non-zero-code when benchmark fails

RUNS=2
PAPI_EXEC_ARGS="--papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS,PAPI_TOT_CYC"

sulong-warmup.json:
	../tools/bench.py -y --filter-runtime="(sulong-warmup-0|sulong-warmup-2)" sulong-warmup.json --filter-harness="(?!(faragon-libsrt|armon-libart))" --runs=1  # TODO: add iterations file

gcc.json:
	../tools/bench.py -y --filter-runtime="gcc" gcc.json --exec-args=${PAPI_EXEC_ARGS} --runs=${RUNS}

clang.json:
	../tools/bench.py -y --filter-runtime="clang" clang.json --exec-args=${PAPI_EXEC_ARGS} --runs=${RUNS}

compcert.json:
	../tools/bench.py -y --filter-runtime="compcert" compcert.json --exec-args=${PAPI_EXEC_ARGS} --runs=${RUNS} --ignore-errors

sanitizers.json:
	../tools/bench.py -y --filter-runtime="(asan-O3|msan-O3|tsan-O3|ubsan-O3)" sanitizers.json --runs=${RUNS} --ignore-errors

tcc.json:
	../tools/bench.py -y --filter-runtime="tcc" tcc.json --exec-args=${PAPI_EXEC_ARGS} --runs=${RUNS} --ignore-errors

valgrind.json:
	../tools/bench.py -y --filter-runtime="valgrind-O3" valgrind.json --runs=${RUNS}

callgrind.json:
	../tools/bench.py -y --filter-runtime="callgrind-O3" callgrind.json --runs=${RUNS}

no-papi.json:
	../tools/bench.py -y --filter-runtime="(gcc|clang)-(O0|O3)" no-papi.json --no-papi --suffix='-no-papi' --runs=${RUNS}

merged.json: gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json sulong-warmup.json no-papi.json
	rm -f merged.json
	../tools/merge_results.py gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json sulong-warmup.json no-papi.json merged.json

Rplots.pdf: merged.json
	../tools/plot.R --benchfile=merged.json --base-runtime=clang-O0
