import XCTest
@testable import Eva

final class LexerBufferCharacterTests: XCTestCase {

  func assertType(
    for c: LexerBuffer.Character,
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
    for _ in 0..<letters.count {
      let c = buffer.nextCharacter()!
      assertType(for: c, isLetter: true, isLowerCaseLetter: true)
    }
  }

  func testIsUpperCaseLetter() {
    let letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    XCTAssertEqual(26, letters.count)

    var buffer = LexerBuffer(expression: letters)
    for _ in 0..<letters.count {
      let c = buffer.nextCharacter()!
      assertType(for: c, isLetter: true, isUpperCaseLetter: true)
    }
  }

  func testIsDigit() {
    let digits = "0123456789"
    XCTAssertEqual(10, digits.count)

    var buffer = LexerBuffer(expression: digits)
    for _ in 0..<digits.count {
      let c = buffer.nextCharacter()!
      assertType(for: c, isDigit: true)
    }
  }

  func testIsSpace() {
    let space = " "
    XCTAssertEqual(1, space.count)
    var buffer = LexerBuffer(expression: space)
    let c = buffer.nextCharacter()!
    assertType(for: c, isSpace: true)
  }

  func testIsAsterisk() {
    let expression = "*"
    XCTAssertEqual(1, expression.count)
    var buffer = LexerBuffer(expression: expression)
    let c = buffer.nextCharacter()!
    assertType(for: c, isAsterisk: true)
  }

  static var allTests = [
      ("testIsLowerCaseLetter", testIsLowerCaseLetter),
      ("testIsUpperCaseLetter", testIsUpperCaseLetter),
      ("testIsDigit", testIsDigit),
      ("testIsSpace", testIsSpace),
      ("testIsAsterisk", testIsAsterisk),
  ]
}
