ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-6.0 libomp-dev
ARG DMALLOC_VERSION="5.5.2"
RUN wget -O - "http://dmalloc.com/releases/dmalloc-${DMALLOC_VERSION}.tgz" | tar -xz -C / && \
    (cd "/dmalloc-${DMALLOC_VERSION}" && ./configure --disable-cxx && make && make install) && rm -r "/dmalloc-${DMALLOC_VERSION}"

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/dmalloc.json", "--filter-runtime=(dmalloc)"] FOREGROUND
