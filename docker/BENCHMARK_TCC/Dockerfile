FROM cicro/benchmark-base as base

FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

# place to store results
VOLUME /data

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends tcc

# our benchmarking project
COPY --from=base /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "TCC=/usr/bin/tcc" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/tcc.json", "--filter-runtime=(tcc)"] FOREGROUND
