GRAAL_GIT_TAG = "vm-1.0.0-rc9"

DOCKER_IMAGE_NAME = "cicro/sulong"
DOCKER_IMAGE_VERSION = $(GRAAL_GIT_TAG)

.PHONY: build

build: Dockerfile
	docker build --build-arg GRAAL_GIT_TAG=$(GRAAL_GIT_TAG) -t $(DOCKER_IMAGE_NAME):$(GRAAL_GIT_TAG) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME):$(GRAAL_GIT_TAG)
