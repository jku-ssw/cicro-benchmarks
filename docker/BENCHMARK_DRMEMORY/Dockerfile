FROM cicro/benchmark-base as base

FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

# place to store results
VOLUME /data

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-6.0 wget
ARG DRMEMORY_URL="https://github.com/DynamoRIO/drmemory/releases/download/release_2.0.1/DrMemory-Linux-2.0.1-2.tar.gz"
RUN wget -O - "${DRMEMORY_URL}" | tar -xz -C /

# our benchmarking project
COPY --from=base /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "DR_MEMORY=/`ls / | grep -i DrMemory`/bin64/drmemory" >> /benchmarks/configs/env && \
    echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/drmemory.json", "--filter-runtime=(drmemory)"] FOREGROUND
