SULONG_GIT_TAG = "master"

DOCKER_IMAGE_NAME = "sulong"
DOCKER_IMAGE_VERSION = $(SULONG_GIT_TAG)

.PHONY: build run

build: Dockerfile
	docker build --build-arg SULONG_GIT_TAG=$(SULONG_GIT_TAG) -t $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) ./
