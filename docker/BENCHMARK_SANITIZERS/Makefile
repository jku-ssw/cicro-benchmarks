GIT_TAG = "master"

DOCKER_IMAGE_NAME = "benchmark-sanitizers"
DOCKER_IMAGE_VERSION = $(GIT_TAG)

DOCKER_RUN_ARGS =

OUTPUT_DIR = $(CURDIR)/data

BENCHMARK_EXEC_ARGS = ""
BENCHMARK_RUNS = 2

.PHONY: build run

build: Dockerfile
	docker build --build-arg GIT_TAG=$(GIT_TAG) -t $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) ./

run:
	docker run -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) --ignore-errors --exec-args=$(BENCHMARK_EXEC_ARGS) --runs=$(BENCHMARK_RUNS)
