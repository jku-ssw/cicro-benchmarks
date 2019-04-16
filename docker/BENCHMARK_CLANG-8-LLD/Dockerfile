ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}


# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends lld-8

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LLVM_PROFDATA=/usr/bin/llvm-profdata-8" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-8.0-lld.json", "--filter-runtime=(clang)", "--suffix=-v8"] FOREGROUND
