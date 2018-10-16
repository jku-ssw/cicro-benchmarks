DOCKER_IMAGE_NAME = "cicro/benchmark-qemu"

DOCKER_RUN_ARGS = --privileged

OUTPUT_DIR = $(CURDIR)/data
OUTPUT_FILE = $(OUTPUT_DIR)/qemu.json

BENCHMARK_ARGS =
BENCHMARK_RUNS = 2

.PHONY: build run

build: Dockerfile
	docker build -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME) --runs=$(BENCHMARK_RUNS) $(BENCHMARK_ARGS) $(OUTPUT_FILE)
