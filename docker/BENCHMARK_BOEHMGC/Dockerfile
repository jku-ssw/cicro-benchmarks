FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install --no-install-recommends -y git build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

RUN apt-get update && apt-get install -y --no-install-recommends wget software-properties-common gpg-agent

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main"
RUN apt-get update && apt-get install -y --no-install-recommends clang-7

# our benchmarking project
COPY --from=base /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-7" >> /benchmarks/configs/env
run echo "GC_LIBRARY_PATH=/usr/local/lib/libgc.so" >> /benchmarks/configs/env

RUN wget https://github.com/ivmai/libatomic_ops/releases/download/v7.6.6/libatomic_ops-7.6.6.tar.gz && tar -xzf libatomic_ops-7.6.6.tar.gz
RUN cd libatomic_ops-7.6.6 && ./configure && make && make install
RUN wget https://github.com/ivmai/bdwgc/releases/download/v7.6.8/gc-7.6.8.tar.gz && tar -xzf gc-7.6.8.tar.gz
RUN cd gc-7.6.8/ && ./configure --enable-redirect-malloc --disable-threads && make && make install
RUN LD_PRELOAD=/usr/local/lib/libgc.so wget --version

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/boehmgc.json", "--filter-runtime=(boehmgc)"] FOREGROUND
