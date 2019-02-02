import XCTest

import EvaTests

var tests = [XCTestCaseEntry]()
tests += EvaTests.allTests()
tests += MinuteFieldTests.allTests()
XCTMain(tests)
