include eva.mk

# ------------------------------------------------------------------------------
# configurations, required by eva.mk
# ------------------------------------------------------------------------------
SRC             = src
CMD             = cmd
FMT_FOLDERS     = ${SRC} ${CMD}

# ------------------------------------------------------------------------------
# libs.
# ------------------------------------------------------------------------------
ADT_LIB         = ${BUILD}/adt_vec.o ${BUILD}/adt_sds.o ${BUILD}/adt_map.o
ALGORITHMS_LIB  = ${BUILD}/algorithms_dancing_links.o
BASE_LIB        = ${BUILD}/base_error.o
CRON_LIB        = ${BUILD}/cron_field.o ${BUILD}/cron_expr.o
RNG_LIB         = ${BUILD}/rng_srng64.o ${BUILD}/rng_srng64_normal.o

ALL_LIBS = ${ADT_LIB} ${BASE_LIB} ${CRON_LIB} ${RNG_LIB} ${ALGORITHMS_LIB}

# ------------------------------------------------------------------------------
# tests.
# ------------------------------------------------------------------------------
ADT_TEST_SUITE  = ${BUILD}/adt_vec_test.o ${BUILD}/adt_sds_test.o \
		  ${BUILD}/adt_map_test.o
ADT_TEST_DEP    = ${ADT_LIB} ${BASE_LIB}
ADT_TEST        = ${ADT_TEST_SUITE} ${ADT_TEST_DEP}

ALGORITHMS_TEST_SUITE   = ${BUILD}/algorithms_dancing_links_test.o
ALGORITHMS_TEST_DEP     = ${ALGORITHMS_LIB} ${BASE_LIB} ${ADT_LIB}
ALGORITHMS_TEST         = ${ALGORITHMS_TEST_SUITE} ${ALGORITHMS_TEST_DEP}

CRON_TEST_SUITE = ${BUILD}/cron_cron_test.o
CRON_TEST_DEP   = ${CRON_LIB}
CRON_TEST       = ${CRON_TEST_SUITE} ${CRON_TEST_DEP}

RNG_TEST_SUITE  = ${BUILD}/rng_srng64_test.o ${BUILD}/rng_srng64_normal_test.o
RNG_TEST_DEP    = ${RNG_LIB}
RNG_TEST        = ${RNG_TEST_SUITE} ${RNG_TEST_DEP}

ALL_TESTS       = ${ADT_TEST} ${CRON_TEST} ${RNG_TEST} \
		  ${ALGORITHMS_TEST}
# ------------------------------------------------------------------------------
# actions.
# ------------------------------------------------------------------------------

.DEFAULT_GOAL   = compile

compile: ${BUILD} ${ALL_LIBS}

libeva: compile ${BUILD}/libeva.a

${BUILD}/libeva.a: ${ALL_LIBS}
	${EVA_AR} $@ $^

${BUILD}/base_%.o: ${SRC}/base/%.c
	${EVA_CC} -o $@ -c $<

${BUILD}/adt_%.o: ${SRC}/adt/%.c
	${EVA_CC} -o $@ -c $<

${BUILD}/algorithms_%.o: ${SRC}/algorithms/%.c
	${EVA_CC} -o $@ -c $<

${BUILD}/cron_%.o: ${SRC}/cron/%.c
	${EVA_CC} -o $@ -c $<

${BUILD}/rng_%.o: ${SRC}/rng/%.c
	${EVA_CC} -o $@ -c $<

# ------------------------------------------------------------------------------
# cmds.
# ------------------------------------------------------------------------------

# alias
c: cron
s: sudoku

cron: compile ${BUILD}/cron
	${EVA_EX} ${BUILD}/cron

${BUILD}/cron: cmd/cron/main.c ${BASE_LIB} ${ADT_LIB} ${CRON_LIB}
	${EVA_LD} -o $@ $^

sudoku: compile ${BUILD}/sudoku
	${EVA_EX} ${BUILD}/sudoku

${BUILD}/sudoku: cmd/sudoku/main.c ${BASE_LIB} ${ADT_LIB} ${ALGORITHMS_LIB}
	${EVA_LD} -o $@ $^

test: compile ${BUILD}/test
	${EVA_EX} ${BUILD}/test

${BUILD}/test: cmd/test/main.c ${ALL_TESTS}
	${EVA_LD} -o $@ $^

# ------------------------------------------------------------------------------
# docker.
# ------------------------------------------------------------------------------

DOCKER        = .docker

docker:
ifeq ($(UNAME), Linux)
	make RELEASE=1 -C . clean compile ${BUILD_RELEASE}/cron && \
	mkdir -p ${DOCKER} && \
		cp ${BUILD_RELEASE}/cron ${DOCKER} && \
 		docker build -t xiejw/cron -f data/Dockerfile.cron ${DOCKER}
else
	@echo "building docker is supported on linux only."
endif

push_docker:
	docker push xiejw/cron
