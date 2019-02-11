import XCTest
@testable import Eva

final class LexerBufferTests: XCTestCase {

  func testNextCharacter() {
    var buffer = LexerBuffer(expression: "abc")
    for i in 0..<3 {
      let character = buffer.nextCharacter()
      guard case let .character(value, index) = character else {
        XCTFail("Unexpected character \(character).")
        return
      }
      XCTAssertEqual(i, index)
      XCTAssertEqual(
        LexerBuffer.ASCII.a.rawValue + LexerBuffer.CodeUnit(i),
        value)
    }
    let character = buffer.nextCharacter()
    guard case .eof(3) = character else {
      XCTFail("Should be eof.")
      return
    }
  }

  func testRollback() {
    var buffer = LexerBuffer(expression: "a")
    var character = buffer.nextCharacter()
    guard case .character(LexerBuffer.ASCII.a.rawValue, 0) = character else {
      XCTFail("Unexpected character \(character).")
      return
    }

    buffer.rollback()
    character = buffer.nextCharacter()
    guard case .character(LexerBuffer.ASCII.a.rawValue, 0) = character else {
      XCTFail("Unexpected character \(character).")
      return
    }

    character = buffer.nextCharacter()
    guard case .eof(1) = character else {
      XCTFail("Should be eof.")
      return
    }
  }

  func testRollbackForEof() {
    var buffer = LexerBuffer(expression: "a")
    var character = buffer.nextCharacter()
    guard case .character(LexerBuffer.ASCII.a.rawValue, 0) = character else {
      XCTFail("Unexpected character \(character).")
      return
    }

    character = buffer.nextCharacter()
    guard case .eof(1) = character else {
      XCTFail("Should be eof.")
      return
    }

    buffer.rollback()
    character = buffer.nextCharacter()
    guard case .eof(1) = character else {
      XCTFail("Should be eof.")
      return
    }
  }

  static var allTests = [
    ("testNextCharacter", testNextCharacter),
    ("testRollback", testRollback),
    ("testRollbackForEof", testRollbackForEof),
  ]
}
