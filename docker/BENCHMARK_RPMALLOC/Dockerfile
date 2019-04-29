ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git python ninja-build

ARG RPMALLOC_GIT_TAG=6dfc6717a2c571a7cb643872166aef83b079e14c

# install our environment for the specific benchmark
RUN git clone https://github.com/rampantpixels/rpmalloc/ && cd rpmalloc && git checkout "${RPMALLOC_GIT_TAG}"
RUN cd rpmalloc && python configure.py && ninja

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "LD_PRELOAD=/rpmalloc/bin/linux/release/x86-64/librpmallocwrap.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/rpmalloc.json", "--filter-runtime=(rpmalloc-O3)"] FOREGROUND
