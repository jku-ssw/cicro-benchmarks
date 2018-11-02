ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN wget -O - http://lcamtuf.coredump.cx/afl/releases/afl-2.52b.tgz | tar -xz
RUN cd afl-2.52b/libdislocator && make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/afl-2.52b/libdislocator/libdislocator.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/libdislocator.json", "--filter-runtime=(libdislocator-O3)"] FOREGROUND
