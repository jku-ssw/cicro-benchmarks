.PHONY: build run

build: Dockerfile
	sudo docker build -t benchmark:test ./

run:
	sudo docker run -i -v $(CURDIR)/data:/data benchmark:test
	sudo chown -R $USER $(CURDIR)/data  # change owner so we can view data
