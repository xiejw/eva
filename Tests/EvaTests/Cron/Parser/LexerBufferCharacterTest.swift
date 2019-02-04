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
    }
  }

  static var allTests = [
      ("testIsLowerCaseLetter", testIsLowerCaseLetter),
      ("testIsUpperCaseLetter", testIsUpperCaseLetter),
  ]
}
