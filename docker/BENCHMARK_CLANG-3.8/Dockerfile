FROM cicro/benchmark-base as base

FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

RUN apt-get update && apt-get install -y --no-install-recommends wget software-properties-common

# place to store results
VOLUME /data

# install our enviroment for the specific benchmark
RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial main restricted"
RUN apt-get update && apt-get install -y --no-install-recommends clang-3.8 libomp-dev libllvm3.8

# our benchmarking project
COPY --from=base /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-3.8" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-3.8.json", "--filter-runtime=(clang)", "--suffix=-v3.8"] FOREGROUND
