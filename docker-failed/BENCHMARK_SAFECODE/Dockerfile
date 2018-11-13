ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends subversion python

RUN mkdir safecode && cd safecode && svn co http://llvm.org/svn/llvm-project/llvm/branches/release_32 llvm
RUN cd safecode/llvm/projects && svn co http://llvm.org/svn/llvm-project/poolalloc/branches/release_32 poolalloc
RUN cd safecode/llvm/projects && svn co http://llvm.org/svn/llvm-project/safecode/branches/release_32 safecode

RUN cd safecode/llvm && ./configure && make tools-only -j 4
RUN cd safecode/llvm/projects/poolalloc && make -j 4
RUN cd safecode/llvm/projects/safecode && make -j 4

# TODO:
# * get build running
# * install project
# * create benchmark file

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark enviroment
RUN echo "" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/safecode.json", "--filter-runtime=(safecode-O3)"] FOREGROUND
