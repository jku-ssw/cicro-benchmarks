ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends tcc

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "TCC=/usr/bin/tcc" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/tcc.json", "--filter-runtime=(tcc)"] FOREGROUND
