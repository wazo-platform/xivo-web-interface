.PHONY: build

build:
	docker build -t wazopbx/xivo-web-interface:stretch -f ../../Dockerfile-stretch ../../

stop:
	docker-compose kill && docker-compose rm -f

start:
	docker-compose run --service-ports --rm tests

