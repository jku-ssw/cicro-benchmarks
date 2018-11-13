ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends file clang-6.0 llvm libomp-dev python3 python3-pip python3-setuptools
RUN pip3 install wllvm

ARG GRAALVM_DIR
COPY ${GRAALVM_DIR} graalvm

# place to store results
VOLUME /data

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" > /benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> /benchmarks/configs/env \
    && echo "LLVM_CC_NAME=clang-6.0" >> /benchmarks/configs/env \
    && echo "LLVM_CXX_NAME=clang-6.0" >> /benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> /benchmarks/configs/env \
    && echo "GRAALVM_DIR=/graalvm/bin" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/graalvm.json", "--filter-runtime=(sulong-graalvm)", "--no-papi"] FOREGROUND
