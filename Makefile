# ------------------------------------------------------------------------------
# configurations.
# ------------------------------------------------------------------------------
SRC           = src
CMD           = cmd
BUILD_BASE    = .build
BUILD         = ${BUILD_BASE}
BUILD_RELEASE = ${BUILD_BASE}_release
DOCKER        = .docker
UNAME         = $(shell uname)

CFLAGS        := -std=c99 -Wall -Werror -pedantic -Wno-c11-extensions ${CFLAGS}
CFLAGS        := ${CFLAGS} -I${SRC}
LDFLAGS       := -lm ${LDFLAGS}

# enable POSIX
ifeq ($(UNAME), Linux)
CFLAGS := ${CFLAGS} -D_POSIX_C_SOURCE=201410L
endif

# enable asan by `make ASAN=1`
ifdef ASAN
	CFLAGS := ${CFLAGS} -g -fsanitize=address -D_ASAN=1
	BUILD  := ${BUILD}_asan
endif

# enable release by `make RELEASE=1`
ifdef RELEASE
  CFLAGS := ${CFLAGS} -DNDEBUG -O2
  BUILD  := ${BUILD}_release

compile: check_release_folder
endif

# compact print with some colors.
EVA_CC    = ${QUIET_CC}${CC} ${CFLAGS}
EVA_LD    = ${QUIET_LD}${CC} ${LDFLAGS} ${CFLAGS}
EVA_EX    = ${QUIET_EX}

CCCOLOR   = "\033[34m"
LINKCOLOR = "\033[34;1m"
SRCCOLOR  = "\033[33m"
BINCOLOR  = "\033[36;1m"
ENDCOLOR  = "\033[0m"

# enable verbose cmd by `make V=1`
ifndef V
QUIET_CC  = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) \
				  $(SRCCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
QUIET_LD  = @printf '    %b %b\n' $(LINKCOLOR)LD$(ENDCOLOR) \
				  $(BINCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
QUIET_EX  = @printf '    %b %b\n' $(LINKCOLOR)EX$(ENDCOLOR) \
				  $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
endif

FMT = docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/workdir xiejw/clang-format \
    /clang-format.sh

# ------------------------------------------------------------------------------
# libs.
# ------------------------------------------------------------------------------
ADT_LIB        = ${BUILD}/adt_vec.o ${BUILD}/adt_sds.o ${BUILD}/adt_map.o
ALGORITHMS_LIB = ${BUILD}/algorithms_dancing_links.o
BASE_LIB       = ${BUILD}/base_error.o
CRON_LIB       = ${BUILD}/cron_field.o ${BUILD}/cron_expr.o
RNG_LIB        = ${BUILD}/rng_srng64.o ${BUILD}/rng_srng64_normal.o

# ------------------------------------------------------------------------------
# tests.
# ------------------------------------------------------------------------------
ADT_TEST_SUITE  = ${BUILD}/adt_vec_test.o ${BUILD}/adt_sds_test.o \
							    ${BUILD}/adt_map_test.o
ADT_TEST_DEP    = ${ADT_LIB} ${BASE_LIB}
ADT_TEST        = ${ADT_TEST_SUITE} ${ADT_TEST_DEP}

ALGORITHMS_TEST_SUITE = ${BUILD}/algorithms_dancing_links_test.o
ALGORITHMS_TEST_DEP   = ${ALGORITHMS_LIB} ${BASE_LIB} ${ADT_LIB}
ALGORITHMS_TEST       = ${ALGORITHMS_TEST_SUITE} ${ALGORITHMS_TEST_DEP}

CRON_TEST_SUITE = ${BUILD}/cron_cron_test.o
CRON_TEST_DEP   = ${CRON_LIB}
CRON_TEST       = ${CRON_TEST_SUITE} ${CRON_TEST_DEP}

RNG_TEST_SUITE  = ${BUILD}/rng_srng64_test.o ${BUILD}/rng_srng64_normal_test.o
RNG_TEST_DEP    = ${RNG_LIB}
RNG_TEST        = ${RNG_TEST_SUITE} ${RNG_TEST_DEP}

# ------------------------------------------------------------------------------
# actions.
# ------------------------------------------------------------------------------
compile: ${BUILD} ${ADT_LIB} ${BASE_LIB} ${CRON_LIB} ${RNG_LIB} \
	       ${ALGORITHMS_LIB}

${BUILD}:
	@mkdir -p ${BUILD}

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

clean:
	rm -rf ${BUILD_BASE}* ${DOCKER}

fmt:
	${FMT} ${SRC}
	${FMT} ${CMD}

check_release_folder:
ifneq (${BUILD}, ${BUILD_RELEASE})
	@echo "release mode cannot mix with other modes, e.g., asan."
	@exit 1
endif

# ------------------------------------------------------------------------------
# cmds.
# ------------------------------------------------------------------------------

cron: compile ${BUILD}/cron
	${EVA_EX} ${BUILD}/cron

${BUILD}/cron: cmd/cron/main.c ${BASE_LIB} ${ADT_LIB} ${CRON_LIB}
	${EVA_LD} -o $@ $^

test: compile ${BUILD}/test
	${EVA_EX} ${BUILD}/test

${BUILD}/test: cmd/test/main.c ${ADT_TEST} ${CRON_TEST} ${RNG_TEST} \
	             ${ALGORITHMS_TEST}
	${EVA_LD} -o $@ $^

dl: compile ${BUILD}/dl
	${EVA_EX} ${BUILD}/dl

${BUILD}/dl: src/algorithms/dancing_links.c ${BASE_LIB} ${ADT_LIB} ${CRON_LIB}
	${EVA_LD} -o $@ $^

# ------------------------------------------------------------------------------
# docker.
# ------------------------------------------------------------------------------
docker:
ifeq ($(UNAME), Linux)
	make RELEASE=1 -C . clean compile ${BUILD_RELEASE}/cron && \
	mkdir -p ${DOCKER} && \
		cp ${BUILD_RELEASE}/cron ${DOCKER} && \
 		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}
else
	@echo "building docker is supported on linux only."
endif

push_docker:
	docker push xiejw/cron
