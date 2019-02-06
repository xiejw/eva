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
    let expectedTokens: [Token] = [
      Token(
        category: .whiteSpaces(length: 2),
        location: TokenLocation(startIndex: 0, length: 2)),
      Token(
        category: .identifier(identifier: "abc"),
        location: TokenLocation(startIndex: 2, length: 3)),
      Token(
        category: .whiteSpaces(length: 1),
        location: TokenLocation(startIndex: 5, length: 1)),
      Token(
        category: .identifier(identifier: "def"),
        location: TokenLocation(startIndex: 6, length: 3)),
      Token(category: .eof, location: TokenLocation(startIndex: 9, length: 1)),
    ]

    for expectedToken in expectedTokens {
      let token = try! lexer.nextToken()
      guard expectedToken == token else {
        XCTFail("token type error. Got: \(token), expected: \(expectedToken).")
        return
      }
    }
  }

  func testNumberToken() {
    let lexer = Lexer(expression: "  123 456")
    let expectedTokens: [Token] = [
      Token(
        category: .whiteSpaces(length: 2),
        location: TokenLocation(startIndex: 0, length: 2)),
      Token(
        category: .number(value: 123),
        location: TokenLocation(startIndex: 2, length: 3)),
      Token(
        category: .whiteSpaces(length: 1),
        location: TokenLocation(startIndex: 5, length: 1)),
      Token(
        category: .number(value: 456),
        location: TokenLocation(startIndex: 6, length: 3)),
      Token(category: .eof, location: TokenLocation(startIndex: 9, length: 1)),
    ]

    for expectedToken in expectedTokens {
      let token = try! lexer.nextToken()
      guard expectedToken == token else {
        XCTFail("token type error. Got: \(token), expected: \(expectedToken).")
        return
      }
    }
  }

  func testAsteriskToken() {
    let lexer = Lexer(expression: "  *")
    let expectedTokens: [Token] = [
      Token(
        category: .whiteSpaces(length: 2),
        location: TokenLocation(startIndex: 0, length: 2)),
      Token(
        category: .asterisk,
        location: TokenLocation(startIndex: 2, length: 1)),
      Token(category: .eof, location: TokenLocation(startIndex: 3, length: 1)),
    ]

    for expectedToken in expectedTokens {
      let token = try! lexer.nextToken()
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
