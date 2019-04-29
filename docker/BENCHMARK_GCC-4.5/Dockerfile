ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# install our environment for the specific benchmark
RUN apt-get update && apt-get install -y gpg dirmngr
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 40976EAF437D05B5
RUN echo "deb [trusted=yes] http://us.archive.ubuntu.com/ubuntu/ precise universe" >> /etc/apt/sources.list \
	&& echo "deb [trusted=yes] http://us.archive.ubuntu.com/ubuntu/ precise main restricted" >> /etc/apt/sources.list
RUN apt-get update && apt-get install -y --no-install-recommends gcc++-4.5

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "GCC=/usr/bin/gcc-4.5" > /benchmarks/configs/env
RUN gcc-4.5 --version

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/gcc-4.5.json", "--filter-runtime=(gcc)", "--suffix=-v4.5"] FOREGROUND
