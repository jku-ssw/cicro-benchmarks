ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends autoconf
RUN wget -O - https://github.com/jemalloc/jemalloc/releases/download/5.1.0/jemalloc-5.1.0.tar.bz2 | tar -xj
RUN cd jemalloc-5.1.0 && ./autogen.sh && ./configure && make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/jemalloc-5.1.0/lib/libjemalloc.so.2" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/jemalloc.json", "--filter-runtime=(jemalloc-O3)"] FOREGROUND
