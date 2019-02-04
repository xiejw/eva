import XCTest
@testable import Eva

final class FieldTests: XCTestCase {
  func testAny() {
       XCTAssertTrue(Field.any ~= 1)
  }

  func testSingleValue() {
       XCTAssertTrue(Field.singleValue(1) ~= 1)
       XCTAssertFalse(Field.singleValue(2) ~= 1)
  }

  static var allTests = [
      ("testAny", testAny),
      ("testSingleValue", testSingleValue),
  ]
}
