GRAALVM_VERSION = "graalvm-ce-1.0.0-rc16"

DOCKER_IMAGE_NAME = "cicro/benchmark-sulong"

DOCKER_RUN_ARGS =

OUTPUT_DIR = $(CURDIR)/data

BENCHMARK_ARGS =
BENCHMARK_RUNS = 2

.PHONY: build run

build: Dockerfile
	docker build --build-arg GRAALVM_VERSION=$(GRAALVM_VERSION) -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME) --runs=$(BENCHMARK_RUNS) $(BENCHMARK_ARGS)
