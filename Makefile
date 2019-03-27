UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS+= -static
endif

BIN=./build

CXXFLAGS += -Wall -std=c++14
CXXFLAGS += -I.

CXXFILES += tools/cron/main.cpp
CXXFILES += lib/Cron/Expression/Expression.cpp
CXXFILES += lib/Support/Error.cpp


default: clean fmt cron
	$(BIN)/cron

fmt:
	find lib -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google
	find tools -type f | grep -E "(.h|.cpp)$$" | xargs clang-format -i -style=Google

clean:
	rm -rf $(BIN)
	mkdir -p $(BIN)

cron:
	clang++ ${CXXFLAGS} ${CXXFILES} -o $(BIN)/$@
