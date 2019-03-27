@testable import Eva
import XCTest

final class LexerBufferCharacterTests: XCTestCase {
    func assertType(
        for c: LexerBuffer.CodeUnit,
        isLetter: Bool = false,
        isLowerCaseLetter: Bool = false,
        isUpperCaseLetter: Bool = false,
        isSpace: Bool = false,
        isDigit: Bool = false,
        isAsterisk: Bool = false
    ) {
        XCTAssertEqual(isLetter, c.isLetter)
        XCTAssertEqual(isLowerCaseLetter, c.isLowerCaseLetter)
        XCTAssertEqual(isUpperCaseLetter, c.isUpperCaseLetter)
        XCTAssertEqual(isSpace, c.isSpace)
        XCTAssertEqual(isDigit, c.isDigit)
        XCTAssertEqual(isAsterisk, c.isAsterisk)
    }

    func testIsLowerCaseLetter() {
        let letters = "abcdefghijklmnopqrstuvwxyz"
        XCTAssertEqual(26, letters.count)

        var buffer = LexerBuffer(expression: letters)
        for i in 0 ..< letters.count {
            guard case let .character(value, index) = buffer.nextCharacter() else {
                XCTFail("Unexpected char.")
                continue
            }
            XCTAssertEqual(i, index)
            assertType(for: value, isLetter: true, isLowerCaseLetter: true)
        }
    }

    func testIsUpperCaseLetter() {
        let letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        XCTAssertEqual(26, letters.count)

        var buffer = LexerBuffer(expression: letters)
        for i in 0 ..< letters.count {
            guard case let .character(value, index) = buffer.nextCharacter() else {
                XCTFail("Unexpected char.")
                continue
            }
            XCTAssertEqual(i, index)
            assertType(for: value, isLetter: true, isUpperCaseLetter: true)
        }
    }

    func testIsDigit() {
        let digits = "0123456789"
        XCTAssertEqual(10, digits.count)

        var buffer = LexerBuffer(expression: digits)
        for i in 0 ..< digits.count {
            guard case let .character(value, index) = buffer.nextCharacter() else {
                XCTFail("Unexpected char.")
                continue
            }
            XCTAssertEqual(i, index)
            assertType(for: value, isDigit: true)
        }
    }

    func testIsSpace() {
        let space = " "
        XCTAssertEqual(1, space.count)
        var buffer = LexerBuffer(expression: space)
        guard case let .character(value, index) = buffer.nextCharacter() else {
            XCTFail("Unexpected char.")
            return
        }
        XCTAssertEqual(0, index)
        assertType(for: value, isSpace: true)
    }

    func testIsAsterisk() {
        let expression = "*"
        XCTAssertEqual(1, expression.count)
        var buffer = LexerBuffer(expression: expression)
        guard case let .character(value, index) = buffer.nextCharacter() else {
            XCTFail("Unexpected char.")
            return
        }
        XCTAssertEqual(0, index)
        assertType(for: value, isAsterisk: true)
    }

    static var allTests = [
        ("testIsLowerCaseLetter", testIsLowerCaseLetter),
        ("testIsUpperCaseLetter", testIsUpperCaseLetter),
        ("testIsDigit", testIsDigit),
        ("testIsSpace", testIsSpace),
        ("testIsAsterisk", testIsAsterisk),
    ]
}
