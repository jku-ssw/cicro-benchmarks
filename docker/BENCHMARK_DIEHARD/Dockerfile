ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git

ARG DIEHARD_GIT_TAG=5a0f8a521a2c7a2c88ed860d5115d2ee30c617ad
RUN git clone --single-branch --recursive https://github.com/emeryberger/DieHard && cd DieHard && git checkout "${DIEHARD_GIT_TAG}"
RUN cd DieHard/src && make linux-gcc-x86-64

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/DieHard/src/libdiehard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/diehard.json", "--filter-runtime=(diehard-O3)"] FOREGROUND
