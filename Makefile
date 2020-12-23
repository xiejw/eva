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
LDFLAGS := ${LDFLAGS}

# enable POSIX
ifeq ($(UNAME), Linux)
CFLAGS := ${CFLAGS} -D_POSIX_C_SOURCE=201410L
LDFLAGS := -lm ${LDFLAGS}
endif

# enable release by `make RELEASE=1`
ifeq (1, $(RELEASE))
  CFLAGS := ${CFLAGS} -DNDEBUG -O2
  BUILD  := ${BUILD}_release
endif

ifeq (1, $(ASAN))
	CFLAGS := ${CFLAGS} -g -fsanitize=address -D_ASAN=1
	BUILD  := ${BUILD}_asan
endif

# compact print with some colors.
EVA_CC  = ${QUIET_CC}${CC}
EVA_LD  = ${QUIET_LD}${CC} ${LDFLAGS}
EVA_EX  = ${QUIET_EX}

CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
ENDCOLOR="\033[0m"

ifndef V
  QUIET_CC = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) $(SRCCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
  QUIET_LD = @printf '    %b %b\n' $(LINKCOLOR)LD$(ENDCOLOR) $(BINCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
  QUIET_EX = @printf '    %b %b\n' $(LINKCOLOR)EX$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
endif

FMT = docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/workdir xiejw/clang-format \
    /clang-format.sh

# ------------------------------------------------------------------------------
# libs.
# ------------------------------------------------------------------------------
ADT_LIB = ${BUILD}/adt_vec.o ${BUILD}/adt_sds.o ${BUILD}/adt_map.o
BASE_LIB = ${BUILD}/base_error.o
CRON_LIB = ${BUILD}/cron_field.o ${BUILD}/cron_expr.o
RNG_LIB = ${BUILD}/rng_srng64.o ${BUILD}/rng_srng64_normal.o

# ------------------------------------------------------------------------------
# tests.
# ------------------------------------------------------------------------------
ADT_TEST_SUITE  = ${BUILD}/adt_vec_test.o ${BUILD}/adt_sds_test.o \
							    ${BUILD}/adt_map_test.o
ADT_TEST_DEP    = ${ADT_LIB} ${BASE_LIB}
ADT_TEST        = ${ADT_TEST_SUITE} ${ADT_TEST_DEP}

CRON_TEST_SUITE = ${BUILD}/cron_test.o
CRON_TEST_DEP   = ${CRON_LIB}
CRON_TEST       = ${CRON_TEST_SUITE} ${CRON_TEST_DEP}

RNG_TEST_SUITE  = ${BUILD}/rng_srng64_test.o ${BUILD}/rng_srng64_normal_test.o
RNG_TEST_DEP    = ${RNG_LIB}
RNG_TEST        = ${RNG_TEST_SUITE} ${RNG_TEST_DEP}

# ------------------------------------------------------------------------------
# actions.
# ------------------------------------------------------------------------------
compile: ${BUILD} ${ADT_LIB} ${BASE_LIB} ${CRON_LIB} ${RNG_LIB}

${BUILD}:
	@mkdir -p ${BUILD}

${BUILD}/base_error.o: ${SRC}/base/error.c ${SRC}/base/error.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_field.o: ${SRC}/cron/field.c ${SRC}/cron/field.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_expr.o: ${SRC}/cron/expr.c ${SRC}/cron/expr.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/cron_test.o: ${SRC}/cron/cron_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_vec.o: ${SRC}/adt/vec.c ${SRC}/adt/vec.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_vec_test.o: ${SRC}/adt/vec_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_sds.o: ${SRC}/adt/sds.c ${SRC}/adt/sds.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_sds_test.o: ${SRC}/adt/sds_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_map.o: ${SRC}/adt/map.c ${SRC}/adt/map.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/adt_map_test.o: ${SRC}/adt/map_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/rng_srng64.o: ${SRC}/rng/srng64.c ${SRC}/rng/srng64.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/rng_srng64_test.o: ${SRC}/rng/srng64_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/rng_srng64_normal.o: ${SRC}/rng/srng64_normal.c ${SRC}/rng/srng64_normal.h
	${EVA_CC} ${CFLAGS} -o $@ -c $<

${BUILD}/rng_srng64_normal_test.o: ${SRC}/rng/srng64_normal_test.c
	${EVA_CC} ${CFLAGS} -o $@ -c $<

clean:
	rm -rf ${BUILD}* ${DOCKER}

fmt:
	${FMT} ${SRC}
	${FMT} ${CMD}

# ------------------------------------------------------------------------------
# cmds.
# ------------------------------------------------------------------------------

cron: compile ${BUILD}/cron
	${EVA_EX} ${BUILD}/cron

${BUILD}/cron: cmd/cron/main.c ${BASE_LIB} ${ADT_LIB} ${CRON_LIB}
	${EVA_LD} ${CFLAGS} -o $@ $^

test: compile ${BUILD}/test
	${EVA_EX} ${BUILD}/test

${BUILD}/test: cmd/test/main.c ${ADT_TEST} ${CRON_TEST} ${RNG_TEST}
	${EVA_LD} ${CFLAGS} -o $@ $^

#
# # Docker related.
# docker: clean release
# 	mkdir -p ${DOCKER} && \
# 		cp ${RELEASE}/cron ${DOCKER} && \
# 		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}
#
# push_docker:
# 	docker push xiejw/cron
