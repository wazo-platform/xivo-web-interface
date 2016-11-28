.PHONY: test-setup distclean

MANAGE_DB_DIR=../../xivo-manage-db/
POSTGRES_DOCKER=$(MANAGE_DB_DIR)/contribs/postgres-test/Dockerfile


test-setup: build-webi
	docker pull n3llyb0y/wait
	docker pull wazopbx/postgres-test

update-db:
	docker build -t xivo/postgres-test -f $(POSTGRES_DOCKER) $(MANAGE_DB_DIR)

build-webi:
	docker build -t xivo/xivo-web-interface ..
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
	docker rmi xivo/postgres-test
	docker rmi xivo-confd-test
