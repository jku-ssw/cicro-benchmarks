ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends python git libomp-dev

ARG SAFECODE_GIT_TAG=7a4fe7379e9bc07cbb663ee7d9f7820b1bacf3c2
RUN git clone --depth 1 --single-branch https://github.com/jtcriswell/safecode-llvm37 && cd safecode-llvm37 && git checkout "${SAFECODE_GIT_TAG}"

RUN sed -i 's#bits/errno.h#errno.h#g' /safecode-llvm37/projects/safecode/runtime/SoftBoundRuntime/*.c  # FIX: error "Never include <bits/errno.h> directly; use <errno.h> instead."
RUN cd safecode-llvm37 && mkdir build && cd build && ../configure --enable-optimized && make -j 4
RUN cd safecode-llvm37/projects/poolalloc && ./configure && make -j4
RUN cd safecode-llvm37/projects/safecode && ./configure && make -j4

# TODO: implement remaining stuff

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "CLANG=/safecode-llvm37/build/Release+Asserts/bin/clang\nSAFECODE_DIR=/safecode-llvm37/build/Release+Asserts" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/safecode.json", "--filter-runtime=(safecode-O3)"] FOREGROUND
