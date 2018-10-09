FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# our main dependencies
RUN apt-get update && apt-get install -y git build-essential cmake libpapi-dev python3 python3-psutil

# our benchmarking project
ARG GIT_TAG=master
RUN git clone https://github.com/jku-ssw/benchmarks && cd /benchmarks && git checkout "${GIT_TAG}"

# place to store results
VOLUME /data

# configure our enviroment for plotting
RUN export DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y r-base littler libssl-dev libcurl4-openssl-dev libxml2-dev

RUN /benchmarks/requirements.R

ENTRYPOINT ["/benchmarks/tools/plot.R"] FOREGROUND
