GIT_TAG = "master"

DOCKER_IMAGE_NAME = "plot"
DOCKER_IMAGE_VERSION = $(GIT_TAG)

DOCKER_RUN_ARGS =

OUTPUT_DIR = $(CURDIR)/data


.PHONY: build run

build: Dockerfile
	docker build --build-arg GIT_TAG=$(GIT_TAG) -t $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) ./

run:
	docker run -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data --workdir=/data $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) $(PLOT_ARGS)
