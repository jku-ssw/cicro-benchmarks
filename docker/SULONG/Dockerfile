FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our dependencies
RUN apt-get update && apt-get install -y --no-install-recommends git wget file python2.7 build-essential llvm clang libc++-dev libc++abi-dev ca-certificates curl

# get graalvm community edition
ARG GRAALVM_CE_VERSION=1.0.0-rc16
ARG GRAALVM_URL="https://github.com/oracle/graal/releases/download/vm-${GRAALVM_CE_VERSION}/graalvm-ce-${GRAALVM_CE_VERSION}-linux-amd64.tar.gz"
RUN mkdir -p /graalvm \
    && curl -SL ${GRAALVM_URL} \
     | tar -xzC /graalvm --strip-components=1
