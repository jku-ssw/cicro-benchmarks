ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git

ARG LOCKFREEMALLOC_GIT_TAG=915f51b282c5a31b18fb9b96ef19da410b00a421

# install our enviroment for the specific benchmark
RUN git clone --single-branch --depth=1 https://github.com/Begun/lockfree-malloc && cd lockfree-malloc && git checkout "${LOCKFREEMALLOC_GIT_TAG}"
RUN cd lockfree-malloc && make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/lockfree-malloc/liblite-malloc-shared.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/lockfree-malloc.json", "--filter-runtime=(lockfree-malloc-O3)"] FOREGROUND
