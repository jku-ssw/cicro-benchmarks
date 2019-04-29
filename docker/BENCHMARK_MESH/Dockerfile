ARG CACHE_TAG=latest

FROM cicro/benchmark-harness:${CACHE_TAG} as harness
FROM cicro/benchmark-clang-base:${CACHE_TAG}

RUN apt-get update && apt-get install -y --no-install-recommends git python

ARG MESH_GIT_TAG=aeb626e7267eb55ec1855f3d261943c6fb298182

RUN git clone --recurse-submodules https://github.com/plasma-umass/Mesh && cd Mesh && git checkout "${MESH_GIT_TAG}"

RUN cd Mesh && ./configure && make && make install

# our benchmarking project
COPY --from=harness /benchmarks /benchmarks

# configure benchmark environment
RUN echo "LD_PRELOAD=libmesh.so" >> /benchmarks/configs/env.run

ENTRYPOINT ["/benchmarks/tools/bench.py", "/data/mesh.json", "--filter-runtime=(mesh-O3)"] FOREGROUND
