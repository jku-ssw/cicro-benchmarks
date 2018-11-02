ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

ARG DRMEMORY_URL="https://github.com/DynamoRIO/drmemory/releases/download/release_2.0.1/DrMemory-Linux-2.0.1-2.tar.gz"
RUN wget -O - "${DRMEMORY_URL}" | tar -xz -C /

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "DR_MEMORY=/`ls / | grep -i DrMemory`/bin64/drmemory" >> /benchmarks/configs/env && \
    echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/drmemory.json", "--filter-runtime=(drmemory)"] FOREGROUND
