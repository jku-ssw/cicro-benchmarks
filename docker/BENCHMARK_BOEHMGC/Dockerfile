ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN wget -O - https://github.com/ivmai/libatomic_ops/releases/download/v7.6.6/libatomic_ops-7.6.6.tar.gz | tar -xz -C / && \
    cd /libatomic_ops-7.6.6 && ./configure && make && make install && rm -r /libatomic_ops-7.6.6
RUN wget -O - https://github.com/ivmai/bdwgc/releases/download/v7.6.8/gc-7.6.8.tar.gz  | tar -xz -C / && \
    cd /gc-7.6.8 && ./configure --enable-redirect-malloc --disable-threads && make && make install && rm -r /gc-7.6.8

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "LD_PRELOAD=/usr/local/lib/libgc.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/boehmgc.json", "--filter-runtime=(boehmgc)"] FOREGROUND
