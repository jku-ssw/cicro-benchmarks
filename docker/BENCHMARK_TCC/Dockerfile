FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends tcc

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "TCC=/usr/bin/tcc" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/tcc.json", "--filter-runtime=(tcc)"] FOREGROUND
