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

/// Tests combinations.
extension CronExpressionNextDateTests {

  func testSimpleCombination() {
    /// The 3rd day of every month. Time is 19:02.
    let exp = CronExpression(
      minute: .singleValue(2),
      hour: .singleValue(19),
      day: .singleValue(3)
    )
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 3
    dateComponents.day = 3
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 19
    dateComponents.minute = 2
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testWeeklyCombination() {
    // Weekly Sunday at 1am.
    let exp = CronExpression(
      hour: .singleValue(1),
      dayOfWeek: .singleValue(CronExpression.DayOfWeek.sunday.rawValue)
    )
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 2
    dateComponents.day = 17
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 1
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }
}


/// Tests invalid expression.
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

  func testDayOfWeekInNearFuture() {
    // The first saturday in March 2019.
    let exp = CronExpression(
      month: .singleValue(3),
      dayOfWeek: .singleValue(CronExpression.DayOfWeek.saturday.rawValue)
    )
    let nextDate = exp.nextDate(from: generateTestDate())

    var dateComponents = DateComponents()
    dateComponents.year = 2019
    dateComponents.month = 3
    dateComponents.day = 2
    dateComponents.timeZone = TimeZone.current
    dateComponents.hour = 0
    dateComponents.minute = 0
    dateComponents.second = 0
    let expected = calendar.date(from: dateComponents)!
    XCTAssertEqual(expected, nextDate)
  }

  func testNonExistDayOfWeek() {
    // The next March 3 (Saturday) afer year 2019 is in year 2029. So, it is too
    // far away (>5 years).
    let exp = CronExpression(
      day: .singleValue(3),
      month: .singleValue(3),
      dayOfWeek: .singleValue(CronExpression.DayOfWeek.saturday.rawValue)
    )
    let nextDate = exp.nextDate(from: generateTestDate())
    XCTAssertNil(nextDate)
  }

  func testDayOfWeekInLongFuture() {
    // The next March 3 (Saturday) afer year 2019 is in year 2029. So, it is too
    // far away (>5 years).
    let exp = CronExpression(
      day: .singleValue(3),
      month: .singleValue(3),
      dayOfWeek: .singleValue(CronExpression.DayOfWeek.saturday.rawValue),
      maxNumYearsToConsider: 20
    )
    let nextDate = exp.nextDate(from: generateTestDate())
    var dateComponents = DateComponents()
    dateComponents.year = 2029
    dateComponents.month = 3
    dateComponents.day = 3
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
    ("testSimpleCombination", testSimpleCombination),
    ("testWeeklyCombination", testWeeklyCombination),
    //
    ("testYearInPast", testYearInPast),
    //
    ("testDayOfWeekInFollowingWeek", testDayOfWeekInFollowingWeek),
    ("testDayOfWeekInNearFuture", testDayOfWeekInNearFuture),
    ("testNonExistDayOfWeek", testNonExistDayOfWeek),
    ("testDayOfWeekInLongFuture", testDayOfWeekInLongFuture),
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
