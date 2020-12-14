SRC        = src
BUILD_BASE = .build
BUILD      = ${BUILD_BASE}
DOCKER     = .docker

CFLAGS := -std=c99 -Wall -Werror -pedantic -Wno-c11-extensions ${CFLAGS}
CFLAGS := ${CFLAGS} -I${SRC}

# enable release by `make RELEASE=1`
ifeq (1, $(RELEASE))
  CFLAGS := ${CFLAGS} -DNDEBUG -O2
  BUILD := ${BUILD}_release
endif

FMT = docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/workdir xiejw/clang-format \
    /clang-format.sh

compile: ${BUILD} ${BUILD}/cron_field.o ${BUILD}/cron_expr.o

${BUILD}:
	mkdir -p ${BUILD}

${BUILD}/cron_field.o: ${SRC}/cron/field.c ${SRC}/cron/field.h
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_expr.o: ${SRC}/cron/expr.c ${SRC}/cron/expr.h
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm -rf ${BUILD} ${DOCKER}

fmt:
	${FMT} ${SRC}

# cron: compile_only
# 	${DEBUG}/cron
#
# # Docker related.
# docker: clean release
# 	mkdir -p ${DOCKER} && \
# 		cp ${RELEASE}/cron ${DOCKER} && \
# 		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}
#
# push_docker:
# 	docker push xiejw/cron
