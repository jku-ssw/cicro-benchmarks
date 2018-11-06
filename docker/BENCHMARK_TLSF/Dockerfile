ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get install -y --no-install-recommends git

ARG TLSF_GIT_TAG=a1f743ffac0305408b39e791e0ffb45f6d9bc777
RUN git clone --depth 1 --single-branch https://github.com/mattconte/tlsf && cd tlsf && git checkout "${TLSF_GIT_TAG}"
RUN cd tlsf && gcc -c -fpic tlsf.c -o tlsf.a && gcc -shared tlsf.a -o tlsf.so


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "LD_PRELOAD=/tlsf/tlsf.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/tlsf.json", "--filter-runtime=(tlsf-O3)"] FOREGROUND
