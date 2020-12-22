BUILD_DIR=.build
CFLAGS:=-std=c99 -O2 -Wall -Werror -pedantic -Wno-c11-extensions

main:
	mkdir -p ${BUILD_DIR} && \
		${CC} ${CFLAGS} -o ${BUILD_DIR}/rng64.o -c rng64.c && \
		${CC} ${CFLAGS} -o ${BUILD_DIR}/rng64normal.o -c rng64normal.c && \
		${CC} ${CFLAGS} -o ${BUILD_DIR}/main main.c ${BUILD_DIR}/rng64.o ${BUILD_DIR}/rng64normal.o && \
		echo "run ${BUILD_DIR}/main"

