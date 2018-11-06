ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

ARG GRAAL_GIT_TAG=vm-1.0.0-rc9

FROM cicro/sulong:${GRAAL_GIT_TAG}

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpapi-dev python3 python3-psutil python3-pip clang-6.0 libomp-dev ca-certificates python3-setuptools
RUN pip3 install wllvm

# place to store results
VOLUME /data

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" > /benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> /benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> /benchmarks/configs/env \
    && echo "SULONG_DIR=/graalvm/sulong" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/sulong.json", "--no-papi"] FOREGROUND
