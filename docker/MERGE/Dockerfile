FROM cicro/benchmark-harness as harness

FROM ubuntu:18.04

# our main dependencies
RUN apt-get update && apt-get install -y --no-install-recommends python3

# place to store results
VOLUME /data

# our benchmarking project
COPY --from=harness /benchmarks/tools/ /benchmarks/tools

ENTRYPOINT ["/benchmarks/tools/merge_results.py"] FOREGROUND
