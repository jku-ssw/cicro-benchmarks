ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial main restricted"
RUN apt-get update && apt-get install -y --no-install-recommends clang-3.8 libomp-dev libllvm3.8

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-3.8" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-3.8.json", "--filter-runtime=(clang)", "--suffix=-v3.8"] FOREGROUND
