ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git python

ARG ARCHIPELAGO_GIT_TAG=12dc727bdb9714dd914e8ba258647bf59f625266

RUN git clone https://github.com/plasma-umass/Archipelago && cd Archipelago && git checkout "${ARCHIPELAGO_GIT_TAG}"

RUN cd Archipelago && make && make install

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
# RUN echo "LD_PRELOAD=libmesh.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/archipelago.json", "--filter-runtime=(archipelago-O3)"] FOREGROUND
