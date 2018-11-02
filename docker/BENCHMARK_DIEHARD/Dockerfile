ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-6.0 libomp-dev
RUN apt-get update && apt-get install -y --no-install-recommends git
RUN git clone --recursive https://github.com/emeryberger/DieHard
RUN cd DieHard/src && make linux-gcc-x86-64


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" >> /benchmarks/configs/env && \
    echo "LD_PRELOAD=/DieHard/src/libdiehard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/diehard.json", "--filter-runtime=(diehard-O3)"] FOREGROUND
