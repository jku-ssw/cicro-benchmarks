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
RUN apt-get update && apt-get install -y gcc-8 libc6-dev ocaml opam m4  # build requirements for compcert
RUN opam init && eval `opam config env` && opam install coq=8.8.0 menhir=20170101

ARG COMPCERT_VERSION="3.3"
RUN wget -O - "http://compcert.inria.fr/release/compcert-${COMPCERT_VERSION}.tgz" | tar -xz -C /
RUN eval `opam config env` && (cd "/CompCert-$COMPCERT_VERSION" && ./configure x86_64-linux && make -j 4 all)
RUN (cd "/CompCert-$COMPCERT_VERSION" && make install)

RUN echo "COMPCERT=/usr/local/bin/ccomp" >> ./benchmarks/configs/env

ENTRYPOINT ["./benchmarks/tools/bench.py", "/data/compcert.json", "--filter-runtime=(compcert)"] FOREGROUND
