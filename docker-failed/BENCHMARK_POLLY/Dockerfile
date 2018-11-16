ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get install -y --no-install-recommends git
RUN mkdir polly && cd polly && wget http://polly.llvm.org/polly.sh && chmod +x polly.sh && ./polly.sh

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/polly.json", "--filter-runtime=(poll-O3)"] FOREGROUND
