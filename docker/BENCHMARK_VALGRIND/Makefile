DOCKER_IMAGE_NAME = "cicro/benchmark-valgrind"

DOCKER_RUN_ARGS = --privileged

OUTPUT_DIR = $(CURDIR)/data
OUTPUT_FILE = /data/valgrind.json

BENCHMARK_ARGS =
BENCHMARK_RUNS = 2

.PHONY: build run

build: Dockerfile
	docker build -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME) --runs=$(BENCHMARK_RUNS) $(BENCHMARK_ARGS) $(OUTPUT_FILE)
