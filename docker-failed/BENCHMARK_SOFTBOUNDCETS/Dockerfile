ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends python git libomp-dev

ARG SOFTBOUND_GIT_TAG=9c4b6c2b1701923304ed3caea97ec6edd524ff38
RUN git clone --depth 1 --single-branch https://github.com/santoshn/softboundcets-3.8.0 && cd softboundcets-3.8.0 && git checkout "${SOFTBOUND_GIT_TAG}"
RUN cd softboundcets-3.8.0/llvm-38 && mkdir build && cd build && cmake .. && make -j4

ENV PATH="/softboundcets-3.8.0/llvm-38/build/bin:${PATH}"
RUN sed -i 's#bits/errno.h#errno.h#g' /softboundcets-3.8.0/runtime/*.c  # FIX: error "Never include <bits/errno.h> directly; use <errno.h> instead."
RUN cd softboundcets-3.8.0/runtime && make -j8

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

RUN ls /softboundcets-3.8.0/runtime

# configure benchmark environment
RUN echo "CLANG=/softboundcets-3.8.0/llvm-38/build/bin/clang" >> /benchmarks/configs/env && \
    echo "SOFTBOUND_RUNTIME_DIR=/softboundcets-3.8.0/runtime" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/softboundcets.json", "--filter-runtime=softboundcets"] FOREGROUND
