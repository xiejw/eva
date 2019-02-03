import XCTest
@testable import Eva

final class CronExpressionNextDateTests: XCTestCase {
  func generateTestDate() -> Date {
    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 11
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 34
    dateComponents.second = 3
    return Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
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
    let expected = Calendar.current.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }
}

extension CronExpressionNextDateTests {
  static var allTests = [
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
