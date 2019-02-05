import XCTest
@testable import Eva

final class LexerBufferCharacterTests: XCTestCase {

  func testIsLowerCaseLetter() {
    let letters = "abcdefghijklmnopqrstuvwxyz"
    XCTAssertEqual(26, letters.count)

    var buffer = LexerBuffer(expression: letters)
    for _ in 0..<letters.count {
      let c = buffer.nextCharacter()!
      XCTAssertTrue(c.isLetter)
      XCTAssertTrue(c.isLowerCaseLetter)
      XCTAssertFalse(c.isUpperCaseLetter)
      XCTAssertFalse(c.isSpace)
      XCTAssertFalse(c.isDigit)
    }
  }

  func testIsUpperCaseLetter() {
    let letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    XCTAssertEqual(26, letters.count)

    var buffer = LexerBuffer(expression: letters)
    for _ in 0..<letters.count {
      let c = buffer.nextCharacter()!
      XCTAssertTrue(c.isLetter)
      XCTAssertFalse(c.isLowerCaseLetter)
      XCTAssertTrue(c.isUpperCaseLetter)
      XCTAssertFalse(c.isSpace)
      XCTAssertFalse(c.isDigit)
    }
  }

  func testIsDigit() {
    let digits = "0123456789"
    XCTAssertEqual(10, digits.count)

    var buffer = LexerBuffer(expression: digits)
    let c = buffer.nextCharacter()!
    XCTAssertTrue(c.isDigit)
    XCTAssertFalse(c.isSpace)
    XCTAssertFalse(c.isLetter)
    XCTAssertFalse(c.isLowerCaseLetter)
    XCTAssertFalse(c.isUpperCaseLetter)
  }

  func testIsSpace() {
    let space = " "
    var buffer = LexerBuffer(expression: space)
    let c = buffer.nextCharacter()!
    XCTAssertTrue(c.isSpace)
    XCTAssertFalse(c.isLetter)
    XCTAssertFalse(c.isLowerCaseLetter)
    XCTAssertFalse(c.isUpperCaseLetter)
    XCTAssertFalse(c.isDigit)
  }

  static var allTests = [
      ("testIsLowerCaseLetter", testIsLowerCaseLetter),
      ("testIsUpperCaseLetter", testIsUpperCaseLetter),
      ("testIsSpace", testIsSpace),
  ]
}
