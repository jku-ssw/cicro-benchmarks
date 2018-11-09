DOCKER_IMAGE_NAME = "cicro/benchmark-graalvm"

GRAALVM_DIR = "graalvm"

DOCKER_RUN_ARGS = --privileged

OUTPUT_DIR = $(CURDIR)/data

BENCHMARK_ARGS =
BENCHMARK_RUNS = 2

.PHONY: build run

build: $(GRAALVM_DIR) Dockerfile
	docker build --build-arg GRAALVM_DIR=$(GRAALVM_DIR) -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME) --runs=$(BENCHMARK_RUNS) $(BENCHMARK_ARGS)
