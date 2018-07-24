.PHONY: build run

build: Dockerfile
	sudo docker build -t benchmark-sulong:test ./

run:
	sudo docker run -i -v $(CURDIR)/data:/data benchmark-sulong:test
	sudo chown -R $USER $(CURDIR)/data  # change owner so we can view data
