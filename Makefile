DEBUG=./.debug
RELEASE=./.release
DOCKER=./.docker

compile:
	mkdir -p ${DEBUG} && cd ${DEBUG} && CLICOLOR_FORCE=1 cmake .. && make -j

compile_only:
	cd ${DEBUG} && make -j --no-print-directory

release:
	mkdir -p ${RELEASE} && \
		cd ${RELEASE} && \
		CLICOLOR_FORCE=1 cmake -DCMAKE_BUILD_TYPE=RELEASE .. && \
		make -j

clean:
	rm -rf ${DEBUG} ${RELEASE} ${DOCKER}

cron: compile_only
	${DEBUG}/cron

scanner: compile_only
	${DEBUG}/scanner

test: compile
	SKIP_LONG_TEST=1 ${DEBUG}/test

test_full: compile
	${DEBUG}/test

fmt:
	docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/source xiejw/clang-format \
    /clang-format.sh .

# Docker related.
docker: clean release
	mkdir -p ${DOCKER} && \
		cp ${RELEASE}/cron ${DOCKER} && \
		docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron ${DOCKER}

push_docker:
	docker push xiejw/cron
