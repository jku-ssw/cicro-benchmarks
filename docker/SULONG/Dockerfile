FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our dependencies
RUN apt-get update && apt-get install -y --no-install-recommends git wget python2.7 build-essential llvm clang libc++-dev libc++abi-dev ca-certificates

# install mx
RUN mkdir graalvm && git clone https://github.com/graalvm/mx graalvm/mx
ENV PATH="/graalvm/mx:${PATH}"

# install JVMCI
ARG JVMCI_URL="https://github.com/graalvm/openjdk8-jvmci-builder/releases/download/jvmci-0.46/openjdk-8u172-jvmci-0.46-linux-amd64.tar.gz"
RUN wget -O - "${JVMCI_URL}" | tar -xz -C /graalvm

# install and build sulong
ARG SULONG_GIT_TAG=master
RUN git clone https://github.com/graalvm/sulong graalvm/sulong && cd /graalvm/sulong && git checkout "${SULONG_GIT_TAG}"
RUN echo "JAVA_HOME=/graalvm/`ls /graalvm | grep jvmci`" > /graalvm/sulong/mx.sulong/env
RUN cd graalvm/sulong && mx build

CMD ["/bin/bash"]
