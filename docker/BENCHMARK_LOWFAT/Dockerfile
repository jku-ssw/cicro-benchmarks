ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-4.0 libomp-dev git

ARG LOWFAT_GIT_TAG=db75435a11378727585256e8621fcb219e21704e
RUN git clone --single-branch https://github.com/GJDuck/LowFat && cd LowFat && git checkout "${LOWFAT_GIT_TAG}"
RUN chmod +x ./LowFat/build.sh && cd LowFat && ./build.sh

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/LowFat/build/bin/clang" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/lowfat.json", "--filter-runtime=(lowfat)"] FOREGROUND
