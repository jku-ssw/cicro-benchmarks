FROM cicro/benchmark-harness as harness

FROM ubuntu:18.04

# place to store results
VOLUME /data

# configure our enviroment for plotting
RUN export DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y --no-install-recommends r-base gcc g++ libcurl4-gnutls-dev littler libssl-dev libxml2-dev

# install R dependencies
COPY --from=harness /benchmarks/requirements.R /benchmarks/
RUN /benchmarks/requirements.R

# our benchmarking project files
COPY --from=base /benchmarks/tools/*.R /benchmarks/tools/

ENTRYPOINT ["/benchmarks/tools/plot.R"] FOREGROUND
