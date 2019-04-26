ARG CACHE_TAG=latest
ARG GRAALVM_VERSION=graalvm-ce-1.0.0-rc16

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

# our main dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends build-essential cmake file clang-6.0 llvm libomp-dev ca-certificates libpapi-dev python3 python3-psutil python3-pip python3-setuptools python3-sqlalchemy python3-dateutil \
	git wget file python2.7 build-essential llvm clang libc++-dev libc++abi-dev ca-certificates curl
RUN pip3 install wllvm

# get graalvm community edition
ARG GRAALVM_CE_VERSION=1.0.0-rc16
ARG GRAALVM_URL="https://github.com/oracle/graal/releases/download/vm-${GRAALVM_CE_VERSION}/graalvm-ce-${GRAALVM_CE_VERSION}-linux-amd64.tar.gz"
RUN mkdir -p /graalvm \
    && curl -SL ${GRAALVM_URL} \
     | tar -xzC /graalvm --strip-components=1

# place to store results
VOLUME /data

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/usr/bin/clang-6.0" > /benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> /benchmarks/configs/env \
    && echo "LLVM_CC_NAME=clang-6.0" >> /benchmarks/configs/env \
    && echo "LLVM_CXX_NAME=clang-6.0" >> /benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> /benchmarks/configs/env \
    && echo "GRAALVM_DIR=/graalvm" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/sulong.json", "--no-papi"] FOREGROUND
