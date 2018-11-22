ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get -y install llvm-7-dev

RUN update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.gold" 20 && \
    update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.bfd" 10 && \
    update-alternatives --config ld

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/clang-7.0-gold-ld.json", "--filter-runtime=(clang-gold-ld)", "--suffix=-v7"] FOREGROUND
