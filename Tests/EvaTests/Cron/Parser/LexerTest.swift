import XCTest
@testable import Eva

final class LexerTests: XCTestCase {
  func testNextCharacter() {
    let lexer = Lexer(expression: "abc")
    XCTAssertEqual(Lexer.ASCII.a.rawValue, lexer.nextCharacter())
  }

  static var allTests = [
      ("testNextCharacter", testNextCharacter),
  ]
}
