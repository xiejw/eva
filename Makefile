FORMATTER := swiftformat

build:
	swift build

test: fmt
	swift test

fmt:
	$(FORMATTER) Sources
	$(FORMATTER) Tests

clean:
	swift package clean

time:
	mkdir -p bin
	clang-format -i test.cpp
	clang++ -Wall -static -std=c++14 test.cpp -o bin/time
