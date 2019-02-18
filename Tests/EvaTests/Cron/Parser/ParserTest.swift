import XCTest
@testable import Eva

final class PaserTests: XCTestCase {

}

extension PaserTests {
  func testInvalidCharacter() {
    let parser = Parser(expression: "#")
    do {
      _ = try parser.parseTopLevelExpression()
      XCTFail("Should fail.")
    } catch let LexerError.unexpectedCharacter(character) {
      let expectedCharater = Character.character(
        value: 35,  // 35 is the code unit for '#'
        index: 0)
      guard expectedCharater == character else {
        XCTFail("Unexpected character: \(character).")
        return
      }
    } catch {
      XCTFail("Unexpected error: \(error).")
    }
  }
}

extension PaserTests {
  static var allTests = [
    ("testInvalidCharacter", testInvalidCharacter),
  ]
}
