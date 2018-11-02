ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

ARG DMALLOC_VERSION="5.5.2"
RUN wget -O - "http://dmalloc.com/releases/dmalloc-${DMALLOC_VERSION}.tgz" | tar -xz -C / && \
    (cd "/dmalloc-${DMALLOC_VERSION}" && ./configure --disable-cxx && make && make install) && rm -r "/dmalloc-${DMALLOC_VERSION}"

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/dmalloc.json", "--filter-runtime=(dmalloc)"] FOREGROUND
