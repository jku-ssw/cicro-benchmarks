ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get install -y --no-install-recommends git unzip
RUN wget -O - https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-linux.zip > ninja-linux.zip && unzip ninja-linux.zip
RUN wget -O - https://github.com/rampantpixels/rpmalloc/archive/1.3.1.tar.gz | tar xz
RUN apt-get -y install python # see https://github.com/rampantpixels/rpmalloc/issues/77
RUN cd rpmalloc-1.3.1 && python configure.py && /ninja

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/rpmalloc-1.3.1/bin/linux/release/x86-64/librpmallocwrap.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/rpmalloc.json", "--filter-runtime=(rpmalloc-O3)"] FOREGROUND
