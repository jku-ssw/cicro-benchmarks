ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends time

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "--filter-runtime=(usr-bin-time-O3)"] FOREGROUND
