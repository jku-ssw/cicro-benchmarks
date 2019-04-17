ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git python

ARG SCALLOC_GIT_TAG=049857919b5fa1d539c9e4206e353daca2e87394

RUN git clone https://github.com/cksystemsgroup/scalloc && cd scalloc && git checkout "${SCALLOC_GIT_TAG}"
RUN cd scalloc && tools/make_deps.sh && build/gyp/gyp --depth=. scalloc.gyp && BUILDTYPE=Release make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/scalloc/out/Release/lib.target/libscalloc.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/scalloc.json", "--filter-runtime=(scalloc-O3)"] FOREGROUND
