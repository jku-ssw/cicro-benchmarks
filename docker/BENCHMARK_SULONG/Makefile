GRAAL_GIT_TAG = "vm-1.0.0-rc9"

DOCKER_IMAGE_NAME = "cicro/benchmark-sulong"

DOCKER_RUN_ARGS =

OUTPUT_DIR = $(CURDIR)/data

BENCHMARK_ARGS =
BENCHMARK_RUNS = 2

.PHONY: build run

build: Dockerfile
	docker build --build-arg GRAAL_GIT_TAG=$(GRAAL_GIT_TAG) -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME) --runs=$(BENCHMARK_RUNS) $(BENCHMARK_ARGS)
