ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends git ed bison flex libomp-dev
ARG ACK_GIT_TAG=2e71c027a35254b4c517e000dcf445c10f597c8a
RUN git clone --single-branch https://github.com/davidgiven/ack && cd ack && git checkout "${ACK_GIT_TAG}"

RUN cd ack && make -j 4 && make install && make clean

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "ACK=/usr/local/bin/ack" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/ack.json", "--filter-runtime=(ack)"] FOREGROUND
