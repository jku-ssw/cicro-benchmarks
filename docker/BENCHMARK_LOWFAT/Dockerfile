FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends clang-4.0 git
RUN git clone https://github.com/GJDuck/LowFat
RUN chmod +x ./LowFat/build.sh && cd LowFat && ./build.sh

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/LowFat/build/bin/clang" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/lowfat.json", "--filter-runtime=(lowfat)"] FOREGROUND
