import XCTest
@testable import Eva

final class LexerTests: XCTestCase {

  func testInvalidCharacter() {
    // FIXME
    // let lexer = Lexer(expression: "#")
    // do {
    //   _ = try lexer.nextToken()
    //   XCTFail("Should fail.")
    // } catch LexerError.unexpectedCharacter(35) {
    //   // Expected. 35 is ASCII value for '#'.
    // } catch {
    //   XCTFail("Unexpected error: \(error).")
    // }
  }

  func testIdentifierToken() {
    let lexer = Lexer(expression: "  abc def")
    var token: TokenCategory
    let expectedTokens: [TokenCategory] = [
      .whiteSpaces(length: 2),
      .identifier(identifier: "abc"),
      .whiteSpaces(length: 1),
      .identifier(identifier: "def"),
      .eof
    ]

    for expectedToken in expectedTokens {
      token = try! lexer.nextToken().category
      guard expectedToken == token else {
        XCTFail("token type error. Got: \(token), expected: \(expectedToken).")
        return
      }
    }
  }

  func testNumberToken() {
    let lexer = Lexer(expression: "  123 456")
    var token: TokenCategory
    let expectedTokens: [TokenCategory] = [
      .whiteSpaces(length: 2),
      .number(value: 123),
      .whiteSpaces(length: 1),
      .number(value: 456),
      .eof
    ]

    for expectedToken in expectedTokens {
      token = try! lexer.nextToken().category
      guard expectedToken == token else {
        XCTFail("token type error. Got: \(token), expected: \(expectedToken).")
        return
      }
    }
  }

  func testAsteriskToken() {
    let lexer = Lexer(expression: "  *")
    var token: TokenCategory
    let expectedTokens: [TokenCategory] = [
      .whiteSpaces(length: 2),
      .asterisk,
      .eof
    ]

    for expectedToken in expectedTokens {
      token = try! lexer.nextToken().category
      guard expectedToken == token else {
        XCTFail("token type error. Got: \(token), expected: \(expectedToken).")
        return
      }
    }
  }

  static var allTests = [
      ("testInvalidCharacter", testInvalidCharacter),
      ("testIdentifierToken", testIdentifierToken),
      ("testNumberToken", testNumberToken),
      ("testAsteriskToken", testAsteriskToken),
  ]
}
