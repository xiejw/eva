UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS += -static
endif

# Folders
BIN = build
LIB = lib
TOOLS = tools
TESTS = tests

CXX = clang++
EVA_CONFIG = ~/Workspace/eva/core/eva-config
CLANG_FORMAT = clang-format -i -style=Google

# CXX Folders
CXX_FOLDERS = ${LIB} ${TOOLS} ${TESTS}

# LLVM related.
LLVM_DIR = ~/Workspace/llvm/
LLVM_LINK = ${LLVM_DIR}/build/bin/llvm-link

# Flags
COMMON_CXXFLAGS += -Wall -std=c++14
COMMON_CXXFLAGS += -I.

CXXFLAGS += ${COMMON_CXXFLAGS}
CXXFLAGS += $(shell ${EVA_CONFIG} --cxxflags gflags)
LDFLAGS += $(shell ${EVA_CONFIG} --ldflags gflags)

# Files
MAIN = ${TOOLS}/cron/main.cpp
SRCS := $(shell find ${LIB} -type f -name '*.cpp')

OBJ_MAIN := $(patsubst %,${BIN}/%,$(MAIN:.cpp=.o))
OBJ_FILES := $(patsubst %,${BIN}/%,$(SRCS:.cpp=.o))

BC_MAIN := $(patsubst %,${BIN}/%,$(MAIN:.cpp=.bc))
BC_FILES := $(patsubst %,${BIN}/%,$(SRCS:.cpp=.bc))

# Tests
TEST_CXXFLAGS += ${COMMON_CXXFLAGS}
TEST_CXXFLAGS += $(shell ${EVA_CONFIG} --cxxflags googletest)
TEST_LDFLAGS += $(shell ${EVA_CONFIG} --ldflags googletest)

TEST_MAIN = tests/main.cpp
TEST_SRCS += $(shell find ${TESTS}/${LIB}/ -type f -name '*.cpp')

TEST_OBJ_FILES := $(patsubst %,${BIN}/%,$(TEST_SRCS:.cpp=.o))
TEST_OBJ_MAIN := $(patsubst %,${BIN}/%,$(TEST_MAIN:.cpp=.o))

default: clean fmt init test cron


# Format the source code.
fmt:
	@for dir in ${CXX_FOLDERS} ; do \
	  find $$dir -type f | grep -E "(.h|.cpp)$$" | xargs ${CLANG_FORMAT} ; \
	done && \
	  echo "Format files."

# For each output file, make sure the folder exists.
init:
	@echo ${OBJ_FILES} ${OBJ_MAIN} ${TEST_OBJ_FILES} ${TEST_OBJ_MAIN} \
	 	| tr ' ' '\n' | xargs -n1 dirname | sort | uniq | xargs -n1 mkdir -p && \
	  echo "Create folders for outputs."

# Remove all outputs.
clean:
	@rm -rf ${BIN} && \
		echo "Delete ${BIN}"

cron: ${OBJ_FILES} ${OBJ_MAIN}
	${CXX} $^ -o $(BIN)/$@ $(LDFLAGS)

test: ${TEST_OBJ_FILES} ${TEST_OBJ_MAIN} ${OBJ_FILES}
	${CXX} $^ -o $(BIN)/$@ $(TEST_LDFLAGS) && ${BIN}/test

bc: ${BC_FILES} ${BC_MAIN}
	${LLVM_LINK} $^ -o ${BIN}/linked.bc

${BIN}/tests/%.o: tests/%.cpp
	${CXX} ${TEST_CXXFLAGS} -c -o $@ $<

${BIN}/%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

${BIN}/%.bc: %.cpp
	${CXX} ${CXXFLAGS} -O0 -emit-llvm -c -o $@ $<

update:
	./utils/update-dependencies

# Docker related.
docker: clean cron
	docker build -t xiejw/cron -f dockerfiles/Dockerfile.cron build

push_docker:
	docker push xiejw/cron
