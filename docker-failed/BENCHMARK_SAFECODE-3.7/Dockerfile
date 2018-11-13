ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends python git libomp-dev

ARG SAFECODE_GIT_TAG=7a4fe7379e9bc07cbb663ee7d9f7820b1bacf3c2
RUN git clone --depth 1 --single-branch https://github.com/jtcriswell/safecode-llvm37 && cd safecode-llvm37 && git checkout "${SAFECODE_GIT_TAG}"
RUN cd safecode-llvm37 && mkdir build && cd build && cmake .. && make -j4

# TODO: implement remaining stuff

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/safecode.json", "--filter-runtime=(safecode-O3)"] FOREGROUND
