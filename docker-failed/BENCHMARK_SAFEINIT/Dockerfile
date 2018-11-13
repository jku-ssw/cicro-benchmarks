ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git clang libomp-dev libc++1

ARG SAFEINIT_GIT_TAG=7c61b86de2eae13277f6ddc271692873bf53bb0e
RUN git clone --single-branch https://github.com/vusec/safeinit.git && cd safeinit && git checkout "${SAFEINIT_GIT_TAG}"
RUN cd safeinit/llvm && mkdir build && cd build && cmake .. -DCMAKE_C_COMPILER=/usr/bin/clang && make -j4

# TODO: implement remaining Dockerfile stuff

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/safeinit.json", "--filter-runtime=(safeinit-O3)"] FOREGROUND
