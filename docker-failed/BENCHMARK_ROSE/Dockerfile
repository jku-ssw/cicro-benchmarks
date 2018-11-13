ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends wget

RUN wget -O - https://raw.githubusercontent.com/rose-compiler/rose-develop/master/scripts/2017-03-ROSE-Unbuntu-16.04-VM-setup.sh | bash

# TODO: implement missing parts

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/rose.json", "--filter-runtime=rose-O3"] FOREGROUND
