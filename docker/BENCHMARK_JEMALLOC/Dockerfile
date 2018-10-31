ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main"
RUN apt-get update && apt-get install -y --no-install-recommends clang-7 libomp-dev
RUN apt-get update && apt-get install -y --no-install-recommends autoconf
RUN wget -O - https://github.com/jemalloc/jemalloc/releases/download/5.1.0/jemalloc-5.1.0.tar.bz2 | tar -xj
RUN cd jemalloc-5.1.0 && ./autogen.sh && ./configure && make


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-7" >> /benchmarks/configs/env && \
    echo "JEMALLOC_PATH=/jemalloc-5.1.0/lib/libjemalloc.so.2" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/jemalloc.json", "--filter-runtime=(jemalloc-O3)"] FOREGROUND
