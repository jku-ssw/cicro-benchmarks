FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y git build-essential cmake libpapi-dev python3 python3-psutil

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

# configure our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y clang-3.9 libomp-dev
RUN echo "CLANG=/usr/bin/clang-3.9" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-3.9.json", "--filter-runtime=(clang)", "--suffix=-v3.9"] FOREGROUND

CMD chmod +rw /data/clang-3.9.json
