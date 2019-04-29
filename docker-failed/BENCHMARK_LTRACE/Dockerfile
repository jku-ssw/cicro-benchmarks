ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends ltrace

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/ltrace.json", "--filter-runtime=(ltrace-O3)"] FOREGROUND
