ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends python git libomp-dev

ARG DANGSAN_GIT_TAG=78006af30db70e42df25b7d44352ec717f6b0802
RUN git clone --depth 1 --single-branch https://github.com/vusec/dangsan && cd dangsan && git checkout "${DANGSAN_GIT_TAG}"

RUN apt-get update && apt-get install -y --no-install-recommends bison build-essential gettext pkg-config ssh subversion  # TODO: check

RUN cd dangsan && FORCE_UNSAFE_CONFIGURE=1 ./autosetup.sh

RUN apt-get update && apt-get install -y --no-install-recommends bison

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/softboundcets.json", "--filter-runtime=softboundcets"] FOREGROUND
