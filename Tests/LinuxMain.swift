import XCTest

import evaTests

var tests = [XCTestCaseEntry]()
tests += evaTests.allTests()
XCTMain(tests)