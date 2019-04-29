ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}


RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ xenial main restricted"

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends bison ccache cmake doxygen exuberant-ctags flex g++-4.9 g++-multilib gcc-4.9 ghc git libantlr3c-dev libboost-dev libboost-filesystem-dev libboost-thread-dev libcln-dev libghc-regex-compat-dev libghc-regex-tdfa-dev libghc-split-dev libjsoncpp-dev python subversion libiml-dev libgmp-dev libboost-regex-dev autoconf libtool antlr pccts pkg-config

RUN git clone --depth 1 --single-branch https://github.com/StanfordPL/stoke  # -b ubuntu16.04

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 100 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 100

RUN cd stoke && ./configure.sh
RUN cd stoke && make z3
RUN cd stoke && make cvc4
RUN cd stoke && bin/stoke --help
RUN cd stoke && bin/stoke extract -i ls -o lsasdf

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "GCC=/usr/bin/gcc-4.9" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-4.9.json", "--filter-runtime=(gcc)", "--suffix=-v4.9"] FOREGROUND
