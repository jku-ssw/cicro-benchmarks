ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness

FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git


RUN git clone https://github.com/llvm-mirror/llvm
RUN cd llvm/tools git clone https://github.com/llvm-mirror/clang

RUN git clone https://github.com/HexHive/HexVASAN.git

RUN rm llvm/tools/clang/lib/Driver/Tools.cpp
RUN ln -s /HexVASAN/src/Tools.cpp llvm/tools/clang/lib/Driver/Tools.cpp
RUN rm llvm/tools/clang/include/clang/Basic/Sanitizers.def
RUN ln -s /HexVASAN/src/Sanitizers.def llvm/tools/clang/include/clang/Basic/Sanitizers.def

RUN rm llvm/tools/clang/include/clang/Driver/SanitizerArgs.h
RUN ln -s /HexVASAN/src/SanitizerArgs.h llvm/tools/clang/include/clang/Driver/SanitizerArgs.h

RUN rm llvm/include/llvm/Transforms/Instrumentation.h
RUN ln -s /HexVASAN/src/Instrumentation.h llvm/include/llvm/Transforms/Instrumentation.h

RUN rm llvm/include/llvm/InitializePasses.h
RUN ln -s /HexVASAN/src/InitializePasses.h llvm/include/llvm/InitializePasses.h

RUN rm llvm/tools/clang/lib/CodeGen/BackendUtil.cpp
RUN ln -s /HexVASAN/src/BackendUtil.cpp llvm/tools/clang/lib/CodeGen/BackendUtil.cpp
