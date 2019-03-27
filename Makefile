UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS+= -static
endif

BIN=./build

CXXFLAGS += -Wall -std=c++14
CXXFLAGS += -I.
CXXFLAGS += -Idependencies/gflags/build/include/
# LDFLAGS += -Ldependencies/gflags/build/lib/
# LDFLAGS += -lgflags -lpthread
LDFLAGS += dependencies/gflags/build/lib/libgflags_nothreads.a

TEST_CXXFLAGS += -Wall -std=c++14
TEST_CXXFLAGS += -I.
TEST_CXXFLAGS += -Idependencies/googletest/googletest/include/
TEST_LDFLAGS += dependencies/googletest/build/lib/libgtest.a -lpthread

MAIN = tools/cron/main.cpp
CXXFILES += lib/Cron/Expression/Expression.cpp
CXXFILES += lib/Support/Error.cpp

TEST_MAIN = tests/main.cpp
TEST_CXXFILES += tests/lib/Support/ErrorTest.cpp

default: clean fmt cron
	$(BIN)/cron

fmt:
	find lib -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google
	find tools -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google

clean:
	rm -rf $(BIN)
	mkdir -p $(BIN)

cron:
	clang++ ${CXXFLAGS} ${CXXFILES} ${MAIN} -o $(BIN)/$@ $(LDFLAGS)

update:
	./utils/update-dependencies

test:
	clang++ ${TEST_CXXFLAGS} ${CXXFILES} ${TEST_CXXFILES} ${TEST_MAIN} \
		-o $(BIN)/$@ $(TEST_LDFLAGS) && \
		${BIN}/test
