FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main"
RUN apt-get update && apt-get install -y --no-install-recommends clang-7 libomp-dev

RUN wget -O - https://github.com/ivmai/libatomic_ops/releases/download/v7.6.6/libatomic_ops-7.6.6.tar.gz | tar -xz -C / && \
    cd /libatomic_ops-7.6.6 && ./configure && make && make install && rm -r /libatomic_ops-7.6.6
RUN wget -O - https://github.com/ivmai/bdwgc/releases/download/v7.6.8/gc-7.6.8.tar.gz  | tar -xz -C / && \
    cd /gc-7.6.8 && ./configure --enable-redirect-malloc --disable-threads && make && make install && rm -r /gc-7.6.8

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-7" >> /benchmarks/configs/env && \
    echo "GC_LIBRARY_PATH=/usr/local/lib/libgc.so" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/boehmgc.json", "--filter-runtime=(boehmgc)"] FOREGROUND
