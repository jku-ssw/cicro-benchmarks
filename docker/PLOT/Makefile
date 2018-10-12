DOCKER_IMAGE_NAME = "plot"

DOCKER_RUN_ARGS =

OUTPUT_DIR = $(CURDIR)/data


.PHONY: build run

build: Dockerfile
	docker build -t $(DOCKER_IMAGE_NAME) ./

run:
	docker run --rm -i $(DOCKER_RUN_ARGS) -v $(OUTPUT_DIR):/data --workdir=/data $(DOCKER_IMAGE_NAME) $(PLOT_ARGS)
