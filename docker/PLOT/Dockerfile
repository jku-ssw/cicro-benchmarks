FROM benchmark-base as base

FROM ubuntu:18.04

MAINTAINER Thomas Pointhuber <thomas.pointhuber@gmx.at>

# place to store results
VOLUME /data

# configure our enviroment for plotting
RUN export DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y r-base littler libssl-dev libcurl4-openssl-dev libxml2-dev

# install R dependencies
COPY --from=base /benchmarks/requirements.R /benchmarks/
RUN /benchmarks/requirements.R

# our benchmarking project files
COPY --from=base /benchmarks/tools/*.R /benchmarks/tools/

ENTRYPOINT ["/benchmarks/tools/plot.R"] FOREGROUND
