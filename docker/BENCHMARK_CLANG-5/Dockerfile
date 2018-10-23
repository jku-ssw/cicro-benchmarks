FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-5.0

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-5.0" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-5.0.json", "--filter-runtime=(clang)", "--suffix=-v5"] FOREGROUND
