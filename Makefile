BIN=./.build
DOCKER=./.docker

compile:
	mkdir -p ${BIN} && cd ${BIN} && CLICOLOR_FORCE=1 cmake .. && make -j

clean:
	rm -rf ${BIN} ${DOCKER}

cron: compile
	${BIN}/cron

scanner: compile
	${BIN}/scanner

test: compile
	${BIN}/test_cron

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
