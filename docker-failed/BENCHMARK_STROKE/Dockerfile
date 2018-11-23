ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends bison ccache cmake doxygen exuberant-ctags flex g++-5 g++-multilib gcc-5 ghc git libantlr3c-dev libboost-dev libboost-filesystem-dev libboost-thread-dev libcln-dev libghc-regex-compat-dev libghc-regex-tdfa-dev libghc-split-dev libjsoncpp-dev python subversion libiml-dev libgmp-dev libboost-regex-dev autoconf libtool antlr pccts pkg-config

RUN git clone --depth 1 --single-branch https://github.com/StanfordPL/stoke -b ubuntu16.04

# git submodule which are not correctly downloaded
RUN cd stoke && ./scripts/make/submodule-init.sh ./src/ext/cpputil &&\
                ./scripts/make/submodule-init.sh ./src/ext/z3
RUN cd stoke && ./scripts/make/submodule-init.sh ./src/ext/x64asm
RUN cd stoke && ./configure.sh && CXX=/usr/bin/g++-5 CC=/usr/bin/gcc-5 make bin/* src/validator/handlers.h bin/stoke_extract -j 4

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "GCC=/usr/bin/gcc-4.9" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-4.9.json", "--filter-runtime=(gcc)", "--suffix=-v4.9"] FOREGROUND
