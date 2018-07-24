.PHONY: build run

build: Dockerfile
	docker build -t benchmark:test ./

run:
	docker run -i -v $(CURDIR)/data:/data benchmark:test
	chown -R $USER $(CURDIR)/data  # change owner so we can view data
