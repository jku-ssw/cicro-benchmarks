FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

RUN apt-get update && apt-get install -y git python3 python3-psutil  # required for benchmark runner
RUN apt-get update && apt-get install -y build-essential cmake gcc-8 clang-6.0 libpapi-dev  # required to build projects

RUN git clone  https://github.com/jku-ssw/benchmarks  # TODO: disable caching for this command?

# configure our enviroment to execute the benchmarks
RUN echo "GCC=/usr/bin/gcc-8" > ./benchmarks/configs/env \
    && echo "CLANG=/usr/bin/clang-6.0" >> ./benchmarks/configs/env

VOLUME /data

ENTRYPOINT ["./benchmarks/tools/bench.py", "/data/benchmark.json", "--filter-runtime=(gcc|clang)-O."] FOREGROUND
