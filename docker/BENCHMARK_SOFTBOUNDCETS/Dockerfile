FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install --no-install-recommends -y git build-essential cmake libpapi-dev python3 python3-psutil ca-certificates

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

RUN apt-get update && apt-get install -y python
RUN git clone https://github.com/santoshn/softboundcets-34/
RUN cd softboundcets-34/softboundcets-llvm-clang34 && chmod +x configure && ./configure --enable-assertions --enable-optimized && make -j8
ENV PATH="/softboundcets-34/softboundcets-llvm-clang34/Release+Asserts/bin:${PATH}"
RUN sed -i 's#bits/errno.h#errno.h#g' /softboundcets-34/softboundcets-lib/softboundcets-wrappers.c
RUN sed -i 's#bits/errno.h#errno.h#g' /softboundcets-34/softboundcets-lib/softboundmpx-wrappers.c
# In file included from softboundcets-wrappers.c:45:
# /usr/include/x86_64-linux-gnu/bits/errno.h:23:3: error: "Never include <bits/errno.h> directly; use <errno.h> instead."
# # error "Never include <bits/errno.h> directly; use <errno.h> instead."
# ^
# 1 error generated.
RUN echo "CLANG=/softboundcets-34/softboundcets-llvm-clang34/Release+Asserts/bin/clang" >> /benchmarks/configs/env && \
    echo "SOFTBOUND_RUNTIME_DIR=/softboundcets-34/softboundcets-lib" >> /benchmarks/configs/env
ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/softboundcets.json", "--filter-runtime=softboundcets"] FOREGROUND
