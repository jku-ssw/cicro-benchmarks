ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-3.9 libomp-dev

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-3.9" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-3.9.json", "--filter-runtime=(clang)", "--suffix=-v3.9"] FOREGROUND
