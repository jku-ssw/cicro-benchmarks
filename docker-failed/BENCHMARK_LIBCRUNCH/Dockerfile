ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}


RUN apt-get update && apt-get install -y bison flex texinfo git
RUN git clone https://github.com/stephenrkell/binutils-gdb.git
RUN cd binutils-gdb && \
   chmod +x configure && \
   CFLAGS="-fPIC -g -O2" ./configure --prefix=/usr/local \
     --enable-gold --enable-plugins --enable-install-libiberty && \
   make -j4 && make install
RUN apt-get update && \
   apt-get install -y libelf-dev \
       autoconf automake libtool pkg-config autoconf-archive \
       g++ ocaml ocaml-findlib \
       default-jre-headless \
       make git gawk gdb \
       libunwind-dev libc6-dev-i386 zlib1g-dev libc6-dbg \
       libboost-iostreams-dev libboost-regex-dev libboost-serialization-dev libboost-filesystem-dev
RUN git clone https://github.com/stephenrkell/liballocs.git
RUN apt-get update && apt-get install -y python
RUN cd liballocs && \
   git submodule init && \
   git submodule update && \
   make -C contrib -j1
RUN ./liballocs/autogen.sh
RUN . ./liballocs/contrib/env.sh && \
   ./liballocs/configure --prefix=/usr/local
RUN make ./liballocs -j4


# our benchmarking project
COPY --from=harness /benchmarks /benchmarks


