BIN=./.build
DOCKER=./.docker

compile:
	mkdir -p ${BIN} && cd ${BIN} && CLICOLOR_FORCE=1 cmake .. && make -j

compile_only:
	cd ${BIN} && make -j --no-print-directory

clean:
	rm -rf ${BIN} ${DOCKER}

cron: compile_only
	${BIN}/cron

scanner: compile_only
	${BIN}/scanner

test: compile
	${BIN}/test

fmt:
	docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/source xiejw/clang-format \
    /clang-format.sh .

# Docker related.
docker: clean compile
	mkdir -p ${DOCKER} && \
		cp ${BIN}/cron ${DOCKER} && \
		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}

push_docker:
	docker push xiejw/cron
