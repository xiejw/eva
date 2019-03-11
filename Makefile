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
