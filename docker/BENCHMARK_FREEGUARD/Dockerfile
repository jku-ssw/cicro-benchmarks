ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git

ARG FREEGUARD_GIT_TAG=bfdf6d9a5b25c3f8f974c6a2ac7bb286e5c2b296
RUN git clone --single-branch https://github.com/UTSASRG/FreeGuard && cd FreeGuard && git checkout "${FREEGUARD_GIT_TAG}"
RUN cd FreeGuard && make SSE2RNG=1

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/FreeGuard/libfreeguard.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/freeguard.json", "--filter-runtime=(freeguard-O3)"] FOREGROUND
