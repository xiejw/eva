BIN=./build

compile:
	mkdir -p ${BIN} && cd ${BIN} && CLICOLOR_FORCE=1 cmake .. && make -j

clean:
	rm -rf ${BIN}

test: compile
	${BIN}/test_main

fmt:
	docker run --rm -ti \
    --user `id -u ${USER}`:`id -g ${USER}` \
    -v `pwd`:/source xiejw/clang-format \
    /clang-format.sh .

# Docker related.
docker: clean cron
	docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron build

push_docker:
	docker push xiejw/cron
