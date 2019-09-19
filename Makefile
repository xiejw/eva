clean:
	rm -rf ${BIN}

# Docker related.
docker: clean cron
	docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron build

push_docker:
	docker push xiejw/cron
