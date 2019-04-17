ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y gpg dirmngr
RUN apt-key adv --keyserver keys.gnupg.net --recv-keys 40976EAF437D05B5 630239CC130E1A7FD81A27B140976EAF437D05B5
RUN apt-add-repository "deb [trusted=yes] http://us.archive.ubuntu.com/ubuntu/ precise universe" && apt-add-repository "deb [trusted=yes] http://us.archive.ubuntu.com/ubuntu/ precise main restricted"
RUN apt-get update && apt-get install -y --no-install-recommends gcc++-4.5

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "GCC=/usr/bin/gcc-4.5" > /benchmarks/configs/env
RUN gcc-4.5 --version

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-4.5.json", "--filter-runtime=(gcc)", "--suffix=-v4.5"] FOREGROUND
