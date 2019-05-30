import XCTest

#if !os(macOS)
    public func allTests() -> [XCTestCaseEntry] {
        return [
            // Parser.
            testCase(ParserTests.allTests),
            testCase(LexerTests.allTests),
            testCase(LexerBufferTests.allTests),
            testCase(LexerBufferCharacterTests.allTests),
        ]
    }
#endif
