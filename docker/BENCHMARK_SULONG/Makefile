.PHONY: build run

build: Dockerfile
	docker build -t benchmark-sulong:test ./

run:
	docker run -i -v $(CURDIR)/data:/data benchmark-sulong:test
	chown -R $USER $(CURDIR)/data  # change owner so we can view data
