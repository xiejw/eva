# ------------------------------------------------------------------------------
# configurations.
# ------------------------------------------------------------------------------
SRC        = src
CMD        = cmd
BUILD_BASE = .build
BUILD      = ${BUILD_BASE}
DOCKER     = .docker
UNAME      = $(shell uname)

CFLAGS := -std=c99 -Wall -Werror -pedantic -Wno-c11-extensions ${CFLAGS}
CFLAGS := ${CFLAGS} -I${SRC}

# enable POSIX
ifeq ($(UNAME), Linux)
CFLAGS := ${CFLAGS} -D_POSIX_SOURCE
endif

# enable release by `make RELEASE=1`
ifeq (1, $(RELEASE))
  CFLAGS := ${CFLAGS} -DNDEBUG -O2
  BUILD := ${BUILD}_release
endif

FMT = docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/workdir xiejw/clang-format \
    /clang-format.sh

# ------------------------------------------------------------------------------
# libs.
# ------------------------------------------------------------------------------
CRON_LIB = ${BUILD}/cron_field.o ${BUILD}/cron_expr.o
ADT_LIB = ${BUILD}/adt_vec.o ${BUILD}/adt_sds.o

# ------------------------------------------------------------------------------
# tests.
# ------------------------------------------------------------------------------
CRON_TEST_SUITE = ${BUILD}/cron_test.o
CRON_TEST_DEP   = ${CRON_LIB}
CRON_TEST       = ${CRON_TEST_SUITE} ${CRON_TEST_DEP}

ADT_TEST_SUITE = ${BUILD}/adt_vec_test.o
ADT_TEST_DEP   = ${ADT_LIB}
ADT_TEST       = ${ADT_TEST_SUITE} ${ADT_TEST_DEP}

# ------------------------------------------------------------------------------
# actions.
# ------------------------------------------------------------------------------
compile: ${BUILD} ${CRON_LIB} ${ADT_LIB}

${BUILD}:
	mkdir -p ${BUILD}

${BUILD}/cron_field.o: ${SRC}/cron/field.c ${SRC}/cron/field.h
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_expr.o: ${SRC}/cron/expr.c ${SRC}/cron/expr.h
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_test.o: ${SRC}/cron/cron_test.c
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_vec.o: ${SRC}/adt/vec.c ${SRC}/adt/vec.h
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_vec_test.o: ${SRC}/adt/vec_test.c
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_sds.o: ${SRC}/adt/sds.c ${SRC}/adt/sds.h
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm -rf ${BUILD} ${DOCKER}

fmt:
	${FMT} ${SRC}
	${FMT} ${CMD}

# ------------------------------------------------------------------------------
# cmds.
# ------------------------------------------------------------------------------

cron: compile ${BUILD}/cron
	${BUILD}/cron

${BUILD}/cron: cmd/cron/main.c ${CRON_LIB}
	${CC} ${CFLAGS} -o $@ $^

test: compile ${BUILD}/test
	${BUILD}/test

${BUILD}/test: cmd/test/main.c ${CRON_TEST} ${ADT_TEST}
	${CC} ${CFLAGS} -o $@ $^

#
# # Docker related.
# docker: clean release
# 	mkdir -p ${DOCKER} && \
# 		cp ${RELEASE}/cron ${DOCKER} && \
# 		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}
#
# push_docker:
# 	docker push xiejw/cron
