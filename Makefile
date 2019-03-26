UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXXFLAGS+= -static
endif

CXXFLAGS+= -Wall -std=c++14
CXXFLAGS+=-I.

fmt:
	find lib -iname *.h -o -iname *.cpp | xargs clang-format -i -style=Google

clean:
	rm -rf bin

time:
	mkdir -p bin
	clang++ ${CXXFLAGS} test.cpp -o bin/time
