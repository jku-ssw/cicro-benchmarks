ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y git wget build-essential libtool flex bison python3-dev unzip perl-doc doxygen texlive libboost-all-dev gdb gcc g++ gfortran autoconf automake libxml2-dev libdwarf-dev graphviz openjdk-8-jdk-headless openjdk-8-jre-headless ghostscript

RUN git clone https://github.com/rose-compiler/rose-develop
RUN mkdir build-rose && cd rose-develop/ && ./build
RUN cd build-rose/ && ../rose-develop/configure --prefix=/home/demo/opt/rose-inst --enable-edg_version=4.12 --with-boost-libdir=/usr/lib/x86_64-linux-gnu --with-boost=/usr --enable-languages=c,c++,fortran
RUN cd build-rose/ && make install-core -j2
# our benchmarking project
COPY --from=harness /benchmarks /benchmarks


ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/rose.json", "--filter-runtime=rose-O3"] FOREGROUND
