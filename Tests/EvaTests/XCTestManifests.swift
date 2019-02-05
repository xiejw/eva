import XCTest

#if !os(macOS)
public func allTests() -> [XCTestCaseEntry] {
  return [
    // Parser.
    testCase(LexerTests.allTests),
    testCase(LexerBufferTests.allTests),
    testCase(LexerBufferCharacterTests.allTests),
    // Cron Expression.
    testCase(FieldTests.allTests),
    testCase(CronExpressionTests.allTests),
    testCase(CronExpressionNextDateTests.allTests),
    testCase(FieldTests.allTests),
  ]
}
#endif
