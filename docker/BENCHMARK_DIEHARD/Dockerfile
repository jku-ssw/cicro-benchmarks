ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git
RUN git clone --depth 1 --single-branch --recursive https://github.com/emeryberger/DieHard
RUN cd DieHard/src && make linux-gcc-x86-64

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/DieHard/src/libdiehard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/diehard.json", "--filter-runtime=(diehard-O3)"] FOREGROUND
