ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y gcc-8

RUN rm /usr/bin/gcov && ln -s /usr/bin/gcov-8 /usr/bin/gcov

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "GCC=/usr/bin/gcc-8" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "--filter-runtime=(gcc)", "--suffix=-v8"] FOREGROUND
