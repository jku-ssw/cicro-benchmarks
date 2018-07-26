GIT_TAG = "master"

DOCKER_IMAGE_NAME = "benchmark-clang-4"
DOCKER_IMAGE_VERSION = $(GIT_TAG)

.PHONY: build run

build: Dockerfile
	docker build --build-arg GIT_TAG=$(GIT_TAG) -t $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION) ./

run:
	docker run -i -v $(CURDIR)/data:/data $(DOCKER_IMAGE_NAME):$(DOCKER_IMAGE_VERSION)
	chown -R $USER $(CURDIR)/data  # change owner so we can view data
