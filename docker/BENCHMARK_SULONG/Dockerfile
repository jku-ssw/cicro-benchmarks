FROM sulong:nightly

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

RUN apt-get update && apt-get install -y git python3 python3-psutil python3-pip  # required for benchmark runner
RUN apt-get update && apt-get install -y build-essential cmake clang-6.0  # required to build projects
RUN pip3 install wllvm

RUN git clone  https://github.com/jku-ssw/benchmarks  # TODO: disable caching for this command?

# configure our enviroment to execute the benchmarks
RUN echo "CLANG=/usr/bin/clang-6.0" > ./benchmarks/configs/env \
    && echo "LLVM_COMPILER=clang" >> ./benchmarks/configs/env \
    && echo "WLLVM_DIR=/usr/local/bin" >> ./benchmarks/configs/env \
    && echo "SULONG_LATEST_CMD=mx -p /graalvm/sulong lli" >> ./benchmarks/configs/env

VOLUME /data

ENTRYPOINT ["./benchmarks/tools/bench.py", "/data/benchmark.json", "--no-papi", "--filter-runtime=sulong-warmup-0"] FOREGROUND
