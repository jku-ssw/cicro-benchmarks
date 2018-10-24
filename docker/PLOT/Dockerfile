FROM cicro/benchmark-harness as harness

FROM cicro/benchmark-base

# configure our enviroment for plotting
RUN export DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y --no-install-recommends r-base gcc g++ libcurl4-gnutls-dev littler libssl-dev libxml2-dev

# install R dependencies
COPY --from=harness /benchmarks/requirements.R /benchmarks/
RUN /benchmarks/requirements.R

# our benchmarking project files
COPY --from=harness /benchmarks/tools/*.R /benchmarks/tools/

ENTRYPOINT ["/benchmarks/tools/plot.R"] FOREGROUND
