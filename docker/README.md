# Docker images

To simplify the execution of the benchmarks in a reproducible manner, as well as to allow benchmarks across
compiler and runtime versions we created a bunch of docker images. Those images are mainly used by the Makefile
located in ```../paper``` to get reproducible benchmark results.

## Basic usage

You can build all docker images in this directory using the Makefile in this directory, or the Makefile of a specific image:

```bash
sudo make build GIT_TAG="master"
```

This will create all docker images which can then be executed by their corresponding Makefile inside the child directory:

```bash
sudo make run GIT_TAG="master"
```

Every Makefile contains some variables for things like position of ```/data``` volume which can be overwritten to fulfil
your needs. The images are intentional very similar to allow docker reuse many parts for various images. Please note it
is recommended to set the GIT_TAG because when using ```master``` docker will not rebuild images if required.

Please note building all images requires about 45 minutes. Furthermore, many images are called with ```--privileged``` to
support measuring hardware counters. This can be deactivated by setting ```DOCKER_RUN_ARGS="" BENCHMARK_EXEC_ARGS=""```
when invoking ```sudo make run```