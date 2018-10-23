FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends valgrind clang-6.0

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "--filter-runtime=(valgrind-O3|callgrind-O3)"] FOREGROUND
