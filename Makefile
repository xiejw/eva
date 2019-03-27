UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS+= -static
endif

BIN=./build

COMMON_CXXFLAGS += -Wall -std=c++14
COMMON_CXXFLAGS += -I.

# Library
CXXFLAGS += ${COMMON_CXXFLAGS}
CXXFLAGS += -Idependencies/gflags/build/include/
LDFLAGS += dependencies/gflags/build/lib/libgflags_nothreads.a

MAIN = tools/cron/main.cpp
CXXFILES += lib/Cron/Expression/Expression.cpp
CXXFILES += lib/Support/Error.cpp

# Tests
TEST_CXXFLAGS += ${COMMON_CXXFLAGS}
TEST_CXXFLAGS += -Idependencies/googletest/googletest/include/
TEST_LDFLAGS += dependencies/googletest/build/lib/libgtest.a -lpthread

TEST_MAIN = tests/main.cpp
TEST_CXXFILES += tests/lib/Support/ErrorTest.cpp
TEST_CXXFILES += tests/lib/Expression/ExpressionTest.cpp

default: fmt cron
	$(BIN)/cron

build:
	mkdir -p ${BIN}

fmt:
	find lib -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google
	find tools -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google
	find tests -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google

clean:
	rm -rf $(BIN)
	mkdir -p $(BIN)

cron: build
	clang++ ${CXXFLAGS} ${CXXFILES} ${MAIN} -o $(BIN)/$@ $(LDFLAGS)

update:
	./utils/update-dependencies

test: build
	clang++ ${TEST_CXXFLAGS} ${CXXFILES} ${TEST_CXXFILES} ${TEST_MAIN} \
		-o $(BIN)/$@ $(TEST_LDFLAGS) && \
		${BIN}/test

docker: clean cron
	docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron build

push_docker:
	docker push xiejw/cron
