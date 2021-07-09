include eva.mk

# ------------------------------------------------------------------------------
# Configurations
# ------------------------------------------------------------------------------
SRC             = src
CMD             = cmd
CFLAGS          += -I${SRC}

# ------------------------------------------------------------------------------
# Configurations, required by eva.mk
# ------------------------------------------------------------------------------
FMT_FOLDERS     = ${SRC} ${CMD}

# ------------------------------------------------------------------------------
# Libs.
# ------------------------------------------------------------------------------
ADT_LIB         = ${BUILD}/adt_vec.o ${BUILD}/adt_sds.o \
                  ${BUILD}/adt_dict.o ${BUILD}/adt_value.o \
		  ${BUILD}/adt_hashing.o
ALGORITHMS_LIB  = ${BUILD}/algorithms_dancing_links.o \
		  ${BUILD}/algorithms_diff.o
BASE_LIB        = ${BUILD}/base_error.o
CRON_LIB        = ${BUILD}/cron_field.o ${BUILD}/cron_expr.o
RNG_LIB         = ${BUILD}/rng_srng64.o ${BUILD}/rng_srng64_normal.o
CRYPTO_LIB      = ${BUILD}/crypto_sha256.o

ALL_LIBS = ${ADT_LIB} ${BASE_LIB} ${CRON_LIB} ${RNG_LIB} ${ALGORITHMS_LIB} \
	   ${CRYPTO_LIB}

# ------------------------------------------------------------------------------
# Tests.
# ------------------------------------------------------------------------------
ADT_TEST_SUITE  = ${BUILD}/adt_vec_test.o ${BUILD}/adt_sds_test.o \
		  ${BUILD}/adt_dict_test.o ${BUILD}/adt_value_test.o
ADT_TEST_DEP    = ${ADT_LIB} ${BASE_LIB}
ADT_TEST        = ${ADT_TEST_SUITE} ${ADT_TEST_DEP}

ALGORITHMS_TEST_SUITE   = ${BUILD}/algorithms_dancing_links_test.o \
                          ${BUILD}/algorithms_diff_test.o
ALGORITHMS_TEST_DEP     = ${ALGORITHMS_LIB} ${BASE_LIB} ${ADT_LIB}
ALGORITHMS_TEST         = ${ALGORITHMS_TEST_SUITE} ${ALGORITHMS_TEST_DEP}

CRON_TEST_SUITE = ${BUILD}/cron_cron_test.o
CRON_TEST_DEP   = ${CRON_LIB}
CRON_TEST       = ${CRON_TEST_SUITE} ${CRON_TEST_DEP}

RNG_TEST_SUITE  = ${BUILD}/rng_srng64_test.o ${BUILD}/rng_srng64_normal_test.o
RNG_TEST_DEP    = ${RNG_LIB}
RNG_TEST        = ${RNG_TEST_SUITE} ${RNG_TEST_DEP}

CRYPTO_TEST_SUITE  = ${BUILD}/crypto_sha256_test.o
CRYPTO_TEST_DEP    = ${CRYPTO_LIB} ${ADT_LIB}
CRYPTO_TEST        = ${CRYPTO_TEST_SUITE} ${CRYPTO_TEST_DEP}

ALL_TESTS       = ${ADT_TEST} ${CRON_TEST} ${RNG_TEST} \
		  ${ALGORITHMS_TEST} ${CRYPTO_TEST}
# ------------------------------------------------------------------------------
# Actions.
# ------------------------------------------------------------------------------

.DEFAULT_GOAL = compile

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

${BUILD}/crypto_%.o: ${SRC}/crypto/%.c
	${EVA_CC} -o $@ -c $<

# ------------------------------------------------------------------------------
# Cmds.
# ------------------------------------------------------------------------------

# alias
c: cron
s: sudoku

compile: $(patsubst ${CMD}/%/main.c,${BUILD}/%,$(wildcard ${CMD}/*/main.c))

$(eval $(call objs,cron,$(BUILD),${BASE_LIB} ${ADT_LIB} ${CRON_LIB}))
$(eval $(call objs,sudoku,$(BUILD),${BASE_LIB} ${ADT_LIB} ${ALGORITHMS_LIB}))
$(eval $(call objs,test,$(BUILD),${ALL_TESTS}))

# ------------------------------------------------------------------------------
# Docker.
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

