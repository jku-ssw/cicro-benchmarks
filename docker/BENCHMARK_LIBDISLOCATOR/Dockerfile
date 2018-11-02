ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-6.0 libomp-dev
RUN wget -O - http://lcamtuf.coredump.cx/afl/releases/afl-2.52b.tgz | tar -xz
RUN cd afl-2.52b/libdislocator && make


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env && \
    echo "LD_PRELOAD=/afl-2.52b/libdislocator.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/libdislocator.json", "--filter-runtime=(libdislocator-O3)"] FOREGROUND
