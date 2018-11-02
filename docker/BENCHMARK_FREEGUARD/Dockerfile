ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main"
RUN apt-get update && apt-get install -y --no-install-recommends clang-7 libomp-dev
RUN apt-get update && apt-get install -y --no-install-recommends git
RUN git clone https://github.com/UTSASRG/FreeGuard
RUN ln -s /usr/bin/clang++-7 /usr/bin/clang++ && cd FreeGuard && make SSE2RNG=1


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-7" >> /benchmarks/configs/env && \
    echo "LD_PRELOAD=/FreeGuard/libfreeguard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/freeguard.json", "--filter-runtime=(freeguard-O3)"] FOREGROUND
