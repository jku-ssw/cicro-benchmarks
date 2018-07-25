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
RUN apt-get update && apt-get install -y tcc
RUN echo "TCC=/usr/bin/tcc" >> ./benchmarks/configs/env

ENTRYPOINT ["./benchmarks/tools/bench.py", "/data/benchmark.json", "--runs=2", "--filter-runtime=(tcc)"] FOREGROUND
