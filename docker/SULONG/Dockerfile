FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our dependencies
RUN apt-get update && apt-get install -y --no-install-recommends git wget file python2.7 build-essential llvm clang libc++-dev libc++abi-dev ca-certificates

# install mx
RUN mkdir graalvm && git clone https://github.com/graalvm/mx graalvm/mx
ENV PATH="/graalvm/mx:${PATH}"

# install JVMCI
ARG JVMCI_URL="https://github.com/graalvm/openjdk8-jvmci-builder/releases/download/jvmci-0.46/openjdk-8u172-jvmci-0.46-linux-amd64.tar.gz"
RUN wget -O - "${JVMCI_URL}" | tar -xz -C /graalvm

# install and build sulong
ARG GRAAL_GIT_TAG=vm-1.0.0-rc9
RUN git clone --depth 1 --single-branch --branch "${GRAAL_GIT_TAG}" https://github.com/oracle/graal.git /graalvm/graal
RUN echo "JAVA_HOME=/graalvm/`ls /graalvm | grep jvmci`" > /graalvm/graal/sulong/mx.sulong/env

RUN cd graalvm/graal/sulong && mx --dynamicimport /compiler build
