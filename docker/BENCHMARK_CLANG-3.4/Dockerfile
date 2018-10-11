FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends git build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

RUN apt-get update && apt-get install -y --no-install-recommends wget software-properties-common

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

# configure our enviroment for the specific benchmark
RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ trusty universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ trusty main restricted"
RUN apt-get update && apt-get install -y clang-3.4 libomp-dev
RUN echo "CLANG=/usr/bin/clang" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-3.4.json", "--filter-runtime=(clang)", "--suffix=-v3.4"] FOREGROUND
