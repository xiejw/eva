BUILD  = .build
SRC    = c
DOCKER = .docker

CFLAGS :=-std=c99 -Wall -Werror -pedantic -Wno-c11-extensions ${CFLAGS}
CFLAGS :=${CFLAGS} -Ic

FMT = docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/workdir xiejw/clang-format \
    /clang-format.sh

compile: init ${BUILD}/cron.o

init: ${BUILD}

${BUILD}:
	@mkdir -p ${BUILD}

${BUILD}/cron.o: ${SRC}/cron/field.c ${SRC}/cron/field.h
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm -rf ${BUILD} ${DOCKER}

cron: compile_only
	${DEBUG}/cron

fmt:
	${FMT} ${SRC}

# Docker related.
docker: clean release
	mkdir -p ${DOCKER} && \
		cp ${RELEASE}/cron ${DOCKER} && \
		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}

push_docker:
	docker push xiejw/cron
