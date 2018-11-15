ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

RUN apt-get install -y --no-install-recommends bison flex

RUN wget -O - ftp://pcc.ludd.ltu.se/pub/pcc/pcc-20170101.tgz | tar -xz -C /
RUN cd pcc-20170101 && ./configure --prefix=/usr --libexecdir=/usr/lib/x86_64-linux-gnu && make && make install
RUN wget -O - ftp://pcc.ludd.ltu.se/pub/pcc-libs/pcc-libs-20170101.tgz | tar -xz -C /
RUN cd pcc-libs-20170101 && ./configure --prefix=/usr --libexecdir=/usr/lib/x86_64-linux-gnu && make

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks


ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/pcc.json", "--filter-runtime=(pcc)"] FOREGROUND
