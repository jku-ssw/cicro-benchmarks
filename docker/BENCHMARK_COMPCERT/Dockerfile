ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends gcc-8 libc6-dev ocaml opam m4  # build requirements for compcert
RUN opam init && eval `opam config env` && opam install coq=8.8.0 menhir=20170101

ARG COMPCERT_VERSION="3.3"
RUN wget -O - "http://compcert.inria.fr/release/compcert-${COMPCERT_VERSION}.tgz" | tar -xz -C /
RUN eval `opam config env` && (cd "/CompCert-$COMPCERT_VERSION" && ./configure x86_64-linux && make -j 4 all)
RUN (cd "/CompCert-$COMPCERT_VERSION" && make install)

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "COMPCERT=/usr/local/bin/ccomp" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/compcert.json", "--filter-runtime=(compcert)"] FOREGROUND
