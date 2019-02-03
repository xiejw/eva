import XCTest
@testable import Eva

final class CronExpressionNextDateTests: XCTestCase {
  func generateTestDate() -> Date {
    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 1
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 34
    return Calendar.current.date(from: dateComponents)!
  }

  func testSearchNextMinuteInSameHour() {
    let exp = CronExpression(minute: .singleValue(40))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 1
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 40
    dateComponents.second = 0
    let expected = Calendar.current.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextMinuteInNextHour() {
    let exp = CronExpression(minute: .singleValue(0))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 1
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 2
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = Calendar.current.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  static var allTests = [
      ("testSearchNextMinuteInSameHour", testSearchNextMinuteInSameHour),
      ("testSearchNextMinuteInNextHour", testSearchNextMinuteInNextHour),
  ]
}
