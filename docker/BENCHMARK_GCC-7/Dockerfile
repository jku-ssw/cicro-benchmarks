FROM cicro/benchmark-base as base

FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

# place to store results
VOLUME /data

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y gcc-7

# our benchmarking project
COPY --from=base /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "GCC=/usr/bin/gcc-7" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-7.json", "--filter-runtime=(gcc)", "--suffix=-v7"] FOREGROUND
