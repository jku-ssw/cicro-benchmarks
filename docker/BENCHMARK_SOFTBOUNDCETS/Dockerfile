FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# install our enviroment for the specific benchmark
RUN apt-get update && apt-get install -y --no-install-recommends python git
RUN git clone https://github.com/santoshn/softboundcets-34/
RUN cd softboundcets-34/softboundcets-llvm-clang34 && chmod +x configure && ./configure --enable-assertions --enable-optimized && make -j8
ENV PATH="/softboundcets-34/softboundcets-llvm-clang34/Release+Asserts/bin:${PATH}"
RUN sed -i 's#bits/errno.h#errno.h#g' /softboundcets-34/softboundcets-lib/softboundcets-wrappers.c && \
    sed -i 's#bits/errno.h#errno.h#g' /softboundcets-34/softboundcets-lib/softboundmpx-wrappers.c
# In file included from softboundcets-wrappers.c:45:
# /usr/include/x86_64-linux-gnu/bits/errno.h:23:3: error: "Never include <bits/errno.h> directly; use <errno.h> instead."
# # error "Never include <bits/errno.h> directly; use <errno.h> instead."
# ^
# 1 error generated.

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "CLANG=/softboundcets-34/softboundcets-llvm-clang34/Release+Asserts/bin/clang" >> /benchmarks/configs/env && \
    echo "SOFTBOUND_RUNTIME_DIR=/softboundcets-34/softboundcets-lib" >> /benchmarks/configs/env

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/softboundcets.json", "--filter-runtime=softboundcets"] FOREGROUND
