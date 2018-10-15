FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install --no-install-recommends -y git build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

RUN apt-get update && apt-get install -y clang-4.0
RUN echo "CLANG=/usr/bin/clang-4.0" >> /benchmarks/configs/env
RUN git clone https://github.com/GJDuck/LowFat
RUN chmod +x ./LowFat/build.sh
RUN cd LowFat && ./build.sh
RUN echo "CLANG=/LowFat/build/bin/clang" >> /benchmarks/configs/env
ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/lowfat.json", "--filter-runtime=(lowfat)"] FOREGROUND
