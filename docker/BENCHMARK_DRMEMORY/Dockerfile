FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install --no-install-recommends -y git build-essential cmake libpapi-dev python3 python3-psutil

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

RUN apt-get install -y clang-6.0
RUN echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env
RUN apt-get install -y wget
ARG DRMEMORY_URL="https://github.com/DynamoRIO/drmemory/releases/download/release_2.0.1/DrMemory-Linux-2.0.1-2.tar.gz"
RUN wget -O - "${DRMEMORY_URL}" | tar -xz -C /
RUN echo "DR_MEMORY=/`ls / | grep -i DrMemory`/bin64/drmemory" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/drmemory.json", "--filter-runtime=(drmemory)"] FOREGROUND
