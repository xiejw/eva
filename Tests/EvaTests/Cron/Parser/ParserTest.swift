@testable import Eva
import XCTest

final class PaserTests: XCTestCase {
    func testMinute() throws {
        let parser = Parser(expression: "1 * * * *")
        let cronExpression = try parser.parseTopLevelExpression().codegen()
        XCTAssertEqual(Field.singleValue(1), cronExpression.minute)

        XCTAssertEqual(Field.any, cronExpression.hour)
        XCTAssertEqual(Field.any, cronExpression.day)
        XCTAssertEqual(Field.any, cronExpression.month)
        XCTAssertEqual(Field.any, cronExpression.year)
        XCTAssertEqual(Field.any, cronExpression.dayOfWeek)
    }

    func testHour() throws {
        let parser = Parser(expression: "* 2 * * *")
        let cronExpression = try parser.parseTopLevelExpression().codegen()
        XCTAssertEqual(Field.singleValue(2), cronExpression.hour)

        XCTAssertEqual(Field.any, cronExpression.minute)
        XCTAssertEqual(Field.any, cronExpression.day)
        XCTAssertEqual(Field.any, cronExpression.month)
        XCTAssertEqual(Field.any, cronExpression.year)
        XCTAssertEqual(Field.any, cronExpression.dayOfWeek)
    }

    func testDay() throws {
        let parser = Parser(expression: "* * 3 * *")
        let cronExpression = try parser.parseTopLevelExpression().codegen()
        XCTAssertEqual(Field.singleValue(3), cronExpression.day)

        XCTAssertEqual(Field.any, cronExpression.minute)
        XCTAssertEqual(Field.any, cronExpression.hour)
        XCTAssertEqual(Field.any, cronExpression.month)
        XCTAssertEqual(Field.any, cronExpression.year)
        XCTAssertEqual(Field.any, cronExpression.dayOfWeek)
    }

    func testMonth() throws {
        let parser = Parser(expression: "* * * 4 *")
        let cronExpression = try parser.parseTopLevelExpression().codegen()
        XCTAssertEqual(Field.singleValue(4), cronExpression.month)

        XCTAssertEqual(Field.any, cronExpression.minute)
        XCTAssertEqual(Field.any, cronExpression.hour)
        XCTAssertEqual(Field.any, cronExpression.day)
        XCTAssertEqual(Field.any, cronExpression.year)
        XCTAssertEqual(Field.any, cronExpression.dayOfWeek)
    }

    func testDayOfWeek() throws {
        let parser = Parser(expression: "* * * * 0")
        let cronExpression = try parser.parseTopLevelExpression().codegen()
        XCTAssertEqual(
            Field.singleValue(CronExpression.DayOfWeek.sunday.rawValue),
            cronExpression.dayOfWeek
        )

        XCTAssertEqual(Field.any, cronExpression.minute)
        XCTAssertEqual(Field.any, cronExpression.hour)
        XCTAssertEqual(Field.any, cronExpression.day)
        XCTAssertEqual(Field.any, cronExpression.month)
        XCTAssertEqual(Field.any, cronExpression.year)
    }
}

extension PaserTests {
    func testInvalidCharacter() {
        let parser = Parser(expression: "#")
        do {
            _ = try parser.parseTopLevelExpression()
            XCTFail("Should fail.")
        } catch let LexerError.unexpectedCharacter(character) {
            let expectedCharater = Character.character(
                value: 35, // 35 is the code unit for '#'
                index: 0
            )
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
        ("testMinute", testMinute),
        ("testHour", testHour),
        ("testDay", testDay),
        ("testMonth", testMonth),
        ("testDayOfWeek", testDayOfWeek),
        ("testInvalidCharacter", testInvalidCharacter),
    ]
}
