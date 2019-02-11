import XCTest
@testable import Eva

final class CronExpressionTests: XCTestCase {
  let calendar = Calendar(identifier: Calendar.Identifier.gregorian)

  func generateTestDate() -> Date {
    var dateComponents = DateComponents()
    dateComponents.year = 1980
    dateComponents.month = 7
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 34
    dateComponents.second = 56
    let date = calendar.date(from: dateComponents)!

    XCTAssertEqual(
      CronExpression.DayOfWeek.friday.rawValue,
      calendar.component(.weekday, from: date)
    )
    return date
  }

  func testMinute() {
    let date = generateTestDate()
    XCTAssertTrue(CronExpression(minute: .singleValue(34)) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(CronExpression(minute: .singleValue(2)) ~= date)
  }

  func testHour() {
    let date = generateTestDate()
    XCTAssertTrue(CronExpression(hour: .singleValue(1)) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(CronExpression(hour: .singleValue(2)) ~= date)
  }

  func testDay() {
    let date = generateTestDate()
    XCTAssertTrue(CronExpression(day: .singleValue(11)) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(CronExpression(day: .singleValue(12)) ~= date)
  }

  func testDayOfWeek() {
    let date = generateTestDate()
    XCTAssertTrue(
      CronExpression(dayOfWeek: .singleValue(
        CronExpression.DayOfWeek.friday.rawValue)
      ) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(
      CronExpression(dayOfWeek: .singleValue(
        CronExpression.DayOfWeek.sunday.rawValue)
      ) ~= date)
  }

  func testMonth() {
    let date = generateTestDate()
    XCTAssertTrue(CronExpression(month: .singleValue(7)) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(CronExpression(month: .singleValue(12)) ~= date)
  }

  func testYear() {
    let date = generateTestDate()
    XCTAssertTrue(CronExpression(year: .singleValue(1980)) ~= date)
    XCTAssertTrue(CronExpression() ~= date)
    XCTAssertFalse(CronExpression(year: .singleValue(12)) ~= date)
  }

  static var allTests = [
    ("testMinute", testMinute),
    ("testHour", testHour),
    ("testDay", testDay),
    ("testDayOfWeek", testDayOfWeek),
    ("testMonth", testMonth),
    ("testYear", testYear),
  ]
}
