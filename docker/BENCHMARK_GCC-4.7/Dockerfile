ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial main restricted"
RUN apt-get update && apt-get install -y --no-install-recommends gcc++-4.7

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "GCC=/usr/bin/gcc-4.7" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-4.7.json", "--filter-runtime=(gcc)", "--suffix=-v4.7"] FOREGROUND
