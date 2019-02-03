import XCTest

#if !os(macOS)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(FieldTests.allTests),
        testCase(CronExpressionTests.allTests),
    ]
}
#endif
