ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get install -y --no-install-recommends git
RUN mkdir polly && cd polly && wget http://polly.llvm.org/polly.sh && chmod +x polly.sh
RUN cd polly && ./polly.sh || cd llvm_build && make # try again with -j1 in case not enough memory is available

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/polly.json", "--filter-runtime=(polly-O3)"] FOREGROUND
