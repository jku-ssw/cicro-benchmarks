ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN wget -O - ftp://g.oswego.edu/pub/misc/malloc.c > malloc.c
RUN gcc -c -fpic malloc.c -o malloc.a && gcc -shared malloc.a -o dlmalloc.so

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "LD_PRELOAD=/dlmalloc.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/dlmalloc.json", "--filter-runtime=(dlmalloc-O3)"] FOREGROUND
