ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y gcc-6

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "GCC=/usr/bin/gcc-6" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-6.json", "--filter-runtime=(gcc)", "--suffix=-v6"] FOREGROUND
