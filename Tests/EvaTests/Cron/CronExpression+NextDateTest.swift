import XCTest
@testable import Eva

final class CronExpressionNextDateTests: XCTestCase {
  let calendar = Calendar(identifier: Calendar.Identifier.gregorian)

  func generateTestDate() -> Date {
    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 34
    dateComponents.second = 3
    let date = calendar.date(from: dateComponents)!

    XCTAssertEqual(
      CronExpression.DayOfWeek.monday.rawValue,
      calendar.component(.weekday, from: date)
    )
    return date
  }
}

/// Tests single field.
extension CronExpressionNextDateTests {
  func testYearInPast() {
    let exp = CronExpression(year: .singleValue(2018))
    let nextDate = exp.nextDate(from: generateTestDate())
    XCTAssertNil(nextDate)
  }
}

/// Tests dayOfWeek.
extension CronExpressionNextDateTests {

  func testDayOfWeekInFollowingWeek() {
    let exp = CronExpression(
      dayOfWeek: .singleValue(CronExpression.DayOfWeek.saturday.rawValue)
    )
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 16
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }
}

/// Tests single field.
extension CronExpressionNextDateTests {

  func testSearchNextYear() {
    let exp = CronExpression(year: .singleValue(2020))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2020
    dateComponents.month = 1
    dateComponents.day = 1
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextMonthInSameYear() {
    let exp = CronExpression(month: .singleValue(7))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 7
    dateComponents.day = 1
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextMonthInNextYear() {
    let exp = CronExpression(month: .singleValue(1))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2020
    dateComponents.month = 1
    dateComponents.day = 1
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }


  func testSearchNextDayInSameMonth() {
    let exp = CronExpression(day: .singleValue(17))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 17
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextDayInNextMonth() {
    let exp = CronExpression(day: .singleValue(10))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 3
    dateComponents.day = 10
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }


  func testSearchNextHourInSameDay() {
    let exp = CronExpression(hour: .singleValue(3))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 3
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextHourInNextDay() {
    let exp = CronExpression(hour: .singleValue(0))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 12
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }


  func testSearchNextMinuteInSameHour() {
    let exp = CronExpression(minute: .singleValue(40))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 40
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testSearchNextMinuteInNextHour() {
    let exp = CronExpression(minute: .singleValue(0))
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 2
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }
}

extension CronExpressionNextDateTests {
  static var allTests = [
    //
    ("testYearInPast", testYearInPast),
    //
    ("testDayOfWeekInFollowingWeek", testDayOfWeekInFollowingWeek),
    //
    ("testSearchNextYear", testSearchNextYear),
    ("testSearchNextMonthInSameYear", testSearchNextMonthInSameYear),
    ("testSearchNextMonthInNextYear", testSearchNextMonthInNextYear),
    ("testSearchNextDayInSameMonth", testSearchNextDayInSameMonth),
    ("testSearchNextDayInNextMonth", testSearchNextDayInNextMonth),
    ("testSearchNextHourInSameDay", testSearchNextHourInSameDay),
    ("testSearchNextHourInNextDay", testSearchNextHourInNextDay),
    ("testSearchNextMinuteInSameHour", testSearchNextMinuteInSameHour),
    ("testSearchNextMinuteInNextHour", testSearchNextMinuteInNextHour),
  ]
}
