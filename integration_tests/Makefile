.PHONY: test-image test-setup distclean

MANAGE_DB_DIR=../../xivo-manage-db/
POSTGRES_DOCKER=$(MANAGE_DB_DIR)/contribs/xivo-postgres/Dockerfile


test-setup: build-postgres build-webi
	docker pull n3llyb0y/wait

build-postgres:
	docker pull postgres:9.1
	docker build -t xivo/xivo-postgres -f $(POSTGRES_DOCKER) $(MANAGE_DB_DIR)

build-webi:
	docker build -t xivo/xivo-web-interface ..

test-image:
	docker build --no-cache -t xivo-webi-test -f Dockerfile ..

test:
	nosetests suite

stop:
	cd assets/funckeys && \
	docker-compose kill && \
	docker-compose rm -f

start:
	cd assets/funckeys && \
	docker-compose run --service-ports --rm tests

distclean:
	docker rmi xivo/xivo-postgres
	docker rmi xivo-confd-test
