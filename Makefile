UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS += -static
endif

BIN = ./build

CXX = clang++
EVA_CONFIG = ~/Workspace/eva/core/eva-config
CLANG_FORMAT = clang-format -i -style=Google

# LLVM related.
LLVM_DIR = ~/Workspace/llvm/
LLVM_LINK = ${LLVM_DIR}/build/bin/llvm-link


COMMON_CXXFLAGS += -Wall -std=c++14
COMMON_CXXFLAGS += -I.

# Library
CXXFLAGS += ${COMMON_CXXFLAGS}
CXXFLAGS += $(shell ${EVA_CONFIG} --cxxflags gflags)
LDFLAGS += $(shell ${EVA_CONFIG} --ldflags gflags)

# CXX Folders
CXX_FOLDERS = lib tools tests

# Files
MAIN = tools/cron/main.cpp
SRCS := $(wildcard lib/*.cpp lib/*/*.cpp lib/*/*/*.cpp)

OBJ_MAIN := $(patsubst %,${BIN}/%,$(MAIN:.cpp=.o))
OBJ_FILES := $(patsubst %,${BIN}/%,$(SRCS:.cpp=.o))

BC_MAIN := $(patsubst %,${BIN}/%,$(MAIN:.cpp=.bc))
BC_FILES := $(patsubst %,${BIN}/%,$(SRCS:.cpp=.bc))

# Tests
TEST_CXXFLAGS += ${COMMON_CXXFLAGS}
TEST_CXXFLAGS += $(shell ${EVA_CONFIG} --cxxflags googletest)
TEST_LDFLAGS += $(shell ${EVA_CONFIG} --ldflags googletest)

TEST_MAIN = tests/main.cpp
TEST_SRCS += $(wildcard tests/lib/*.cpp tests/lib/*/*.cpp tests/lib/*/*/*.cpp)

TEST_OBJ_FILES := $(patsubst %,${BIN}/%,$(TEST_SRCS:.cpp=.o))
TEST_OBJ_MAIN := $(patsubst %,${BIN}/%,$(TEST_MAIN:.cpp=.o))

default: clean fmt test cron
	$(BIN)/cron

fmt:
	for dir in ${CXX_FOLDERS} ; do \
	  find $$dir -type f | grep -E "(.h|.cpp)$$" | xargs ${CLANG_FORMAT} ; \
	done

clean:
	rm -rf ${BIN}

cron: ${OBJ_FILES} ${OBJ_MAIN}
	${CXX} $^ -o $(BIN)/$@ $(LDFLAGS)

test: ${TEST_OBJ_FILES} ${TEST_OBJ_MAIN} ${OBJ_FILES}
	${CXX} $^ -o $(BIN)/$@ $(TEST_LDFLAGS) && ${BIN}/test

bc: ${BC_FILES} ${BC_MAIN}
	${LLVM_LINK} $^ -o ${BIN}/linked.bc

${BIN}/tests/%.o: tests/%.cpp
	mkdir -p `dirname $@` && ${CXX} ${TEST_CXXFLAGS} -c -o $@ $<

${BIN}/%.o: %.cpp
	mkdir -p `dirname $@` && ${CXX} ${CXXFLAGS} -c -o $@ $<

${BIN}/%.bc: %.cpp
	mkdir -p `dirname $@` && ${CXX} ${CXXFLAGS} -O0 -emit-llvm -c -o $@ $<

update:
	./utils/update-dependencies

docker: clean cron
	docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron build

push_docker:
	docker push xiejw/cron
