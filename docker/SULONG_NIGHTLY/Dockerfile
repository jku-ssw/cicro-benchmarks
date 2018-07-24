FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our dependencies
RUN apt-get update && apt-get install -y git wget python2.7 build-essential llvm clang libc++-dev libc++abi-dev

# install mx
RUN mkdir graalvm && git clone https://github.com/graalvm/mx graalvm/mx
ENV PATH="/graalvm/mx:${PATH}"

# install JVMCI
RUN wget -O - https://github.com/graalvm/openjdk8-jvmci-builder/releases/download/jvmci-0.46/openjdk-8u172-jvmci-0.46-linux-amd64.tar.gz | tar -xz -C /graalvm

# install and build sulong
RUN git clone https://github.com/graalvm/sulong graalvm/sulong # && cd /graalvm/sulong && git checkout master
RUN echo "JAVA_HOME=/graalvm/openjdk1.8.0_172-jvmci-0.46" > graalvm/sulong/mx.sulong/env
RUN cd graalvm/sulong && mx build

CMD ["/bin/bash"]
