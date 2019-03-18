ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y gcc-8
RUN apt-get update && apt-get install -y git

RUN git clone --depth 1 https://github.com/introspection-libc/safe-libc
RUN gcc -Wall -mmpx -fcheck-pointer-bounds -fPIC -O3 -g -D_POSIX_C_SOURCE=200809L /safe-libc/libc.c -c -o libc-mpx.o && \
    gcc -Wall -mmpx -fcheck-pointer-bounds -fPIC -O3 -g -D_POSIX_C_SOURCE=200809L /safe-libc/mpx.c -c -o mpx.o

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "GCC=/usr/bin/gcc-8" > /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "--filter-runtime=(introspection)", "--suffix=-v8"] FOREGROUND
