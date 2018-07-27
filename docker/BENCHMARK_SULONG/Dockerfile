ARG SULONG_GIT_TAG=master
FROM sulong:"${SULONG_GIT_TAG}"

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y git build-essential cmake libpapi-dev python3 python3-psutil python3-pip clang-6.0
RUN pip3 install wllvm

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

# configure our enviroment to execute the benchmarks
RUN echo "CLANG=/usr/bin/clang-6.0" > ./benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> ./benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> ./benchmarks/configs/env \
    && echo "SULONG_DIR=/graalvm/sulong" >> ./benchmarks/configs/env

ENTRYPOINT ["./benchmarks/tools/bench.py", "/data/sulong.json", "--no-papi", "--filter-runtime=sulong-warmup-0"] FOREGROUND
