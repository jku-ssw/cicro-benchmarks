ARG CACHE_TAG=latest

FROM cicro/benchmark-base:${CACHE_TAG}

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-8 main"
RUN apt-get update && apt-get install -y --no-install-recommends clang-8 libomp-dev

RUN ln -s /usr/bin/clang++-8 /usr/bin/clang++
RUN ln -s /usr/bin/clang-8 /usr/bin/clang
RUN mkdir -p /benchmarks/configs
RUN echo "CLANG=/usr/bin/clang-8" >> /benchmarks/configs/env
