ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends build-essential curl libcap-dev git cmake libncurses5-dev python-minimal python-pip unzip wget libtcmalloc-minimal4 libgoogle-perftools-dev libomp-dev python3 python3-pip python3-setuptools file
RUN pip3 install wllvm

RUN apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ trusty universe" && apt-add-repository "deb http://us.archive.ubuntu.com/ubuntu/ trusty main restricted"

RUN apt-get update && apt-get install -y --no-install-recommends clang-3.4 llvm-3.4 llvm-3.4-dev #llvm-3.4-tools 
RUN ln -sf /usr/bin/llvm-config-3.4 /usr/bin/llvm-config
RUN ln -sf /usr/bin/llvm-link-3.4 /usr/bin/llvm-link

# build STP
RUN apt-get update && apt-get install -y --no-install-recommends cmake bison flex libboost-all-dev python perl zlib1g-dev

RUN git clone --depth 1 --single-branch https://github.com/stp/minisat.git
RUN mkdir minisat/build && cd minisat/build && cmake -DSTATIC_BINARIES=ON -DCMAKE_INSTALL_PREFIX=/usr/local/ ../ && make install

RUN git clone --depth 1 --single-branch --branch 2.1.2 https://github.com/stp/stp.git
RUN mkdir stp/build && cd stp/build && cmake -DBUILD_SHARED_LIBS:BOOL=OFF -DENABLE_PYTHON_INTERFACE:BOOL=OFF .. && make && make install

# build uclibc
RUN git clone https://github.com/klee/klee-uclibc.git  
RUN cd klee-uclibc && ./configure --make-llvm-lib && make -j4

# build KLEE
RUN git clone --depth 1 --single-branch https://github.com/klee/klee.git
RUN mkdir klee_build_dir && cd klee_build_dir && CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" cmake -DENABLE_SOLVER_STP=ON -DENABLE_SYSTEM_TESTS=OFF -DENABLE_POSIX_RUNTIME=ON -DENABLE_KLEE_UCLIBC=ON -DKLEE_UCLIBC_PATH=/klee-uclibc /klee && make -j4

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang" >> /benchmarks/configs/env \
    && echo "KLEE=/klee_build_dir/bin/klee" >> /benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> /benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/klee.json", "--filter-runtime=klee"] FOREGROUND
