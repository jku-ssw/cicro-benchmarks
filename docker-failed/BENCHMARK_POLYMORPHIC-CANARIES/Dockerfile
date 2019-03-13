ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git clang libomp-dev libc++1

RUN git clone https://github.com/zhilongwang/PolymorphicCanaries
RUN cd /PolymorphicCanaries/Runtime\ Environment/ && sed -i 's/LIBPolymorphicCanaries/LIBPolymorphicCanaries.h/g' LIBPolymorphicCanaries.cpp # see https://github.com/zhilongwang/PolymorphicCanaries/pull/1
RUN cd /PolymorphicCanaries/Runtime\ Environment && make
RUN cd /PolymorphicCanaries/Compiler\ based\ Implementation/ && mkdir build && cd build && cmake .. && make

# TODO: instructions unclear

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/polymorphiccanaries.json", "--filter-runtime=(polymorphiccanaries-O3)"] FOREGROUND
