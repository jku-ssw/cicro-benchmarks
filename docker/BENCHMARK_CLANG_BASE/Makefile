DOCKER_IMAGE_NAME = "cicro/benchmark-clang-base"

.PHONY: build pull

build: Dockerfile
	docker build -t $(DOCKER_IMAGE_NAME) ./

pull:
	docker pull $(DOCKER_IMAGE_NAME)
