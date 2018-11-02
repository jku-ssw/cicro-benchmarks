ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends valgrind

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "--filter-runtime=(valgrind-O3|callgrind-O3)"] FOREGROUND
