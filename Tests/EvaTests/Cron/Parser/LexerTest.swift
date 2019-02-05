import XCTest
@testable import Eva

final class LexerTests: XCTestCase {
  func testIdentifiderLexeme() {
    let lexer = Lexer(expression: " abc def")
    var token = lexer.nextToken()
    guard case .identifier(let identifier1) = token else {
      XCTFail("token type error.")
      return
    }
    XCTAssertEqual("abc", identifier1)

    token = lexer.nextToken()
    guard case .identifier(let identifier2) = token else {
      XCTFail("token \(token) type error.")
      return
    }
    XCTAssertEqual("def", identifier2)
  }

  static var allTests = [
      ("testIdentifiderLexeme", testIdentifiderLexeme),
  ]
}
