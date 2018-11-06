ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git
RUN git clone --depth 1 --single-branch https://github.com/emeryberger/Hoard
RUN sed -i 's/fprintf(stderr, versionMessage);//g' Hoard/src/source/libhoard.cpp  # replace version message output to stderr
RUN cd Hoard/src && make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/Hoard/src/libhoard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/hoard.json", "--filter-runtime=(hoard-O3)"] FOREGROUND
