# Cicro Benchmarking Suite

[![Build Status](https://travis-ci.org/jku-ssw/cicro-benchmarks.svg?branch=master)](https://travis-ci.org/jku-ssw/cicro-benchmarks)
[![Docker Hub](https://img.shields.io/badge/docker--hub-automated-blue.svg)](https://hub.docker.com/u/cicro/)

Cicro is a benchmarking suite that consists of C benchmarks that we derived from GitHub projects. All benchmarks are executed in Docker containers to facilitate reproducible research.

The Docker images are located in the ```docker``` directory. For tools for which we failed to create Docker images, erroneous images can be found in the ```docker-failed``` directory.

To build and execute all Docker containers, ```make``` can be invoked in the ```paper``` directory. Note that it is also possible to benchmark single tools. For example, to benchmark GCC 8, use the command ```make data/gcc-8.json```.

The benchmarks can also be executed outside a Docker container, which is recommended only for debugging tools or benchmarks. Consult the Docker images and the Makefile ```paper/Makefile``` to see how the setup the tool to be benchmarked and how to invoke the benchmark runner.
