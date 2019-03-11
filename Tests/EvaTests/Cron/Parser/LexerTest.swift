@testable import Eva
import XCTest

final class LexerTests: XCTestCase {
    func testInvalidCharacter() {
        let lexer = Lexer(expression: "#")
        do {
            _ = try lexer.nextToken()
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

    func testIdentifierToken() {
        let lexer = Lexer(expression: "  abc def")
        let expectedTokens: [Token] = [
            Token(
                category: .whiteSpaces(length: 2),
                location: TokenLocation(startIndex: 0, length: 2)
            ),
            Token(
                category: .identifier(identifier: "abc"),
                location: TokenLocation(startIndex: 2, length: 3)
            ),
            Token(
                category: .whiteSpaces(length: 1),
                location: TokenLocation(startIndex: 5, length: 1)
            ),
            Token(
                category: .identifier(identifier: "def"),
                location: TokenLocation(startIndex: 6, length: 3)
            ),
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
                location: TokenLocation(startIndex: 0, length: 2)
            ),
            Token(
                category: .number(value: 123),
                location: TokenLocation(startIndex: 2, length: 3)
            ),
            Token(
                category: .whiteSpaces(length: 1),
                location: TokenLocation(startIndex: 5, length: 1)
            ),
            Token(
                category: .number(value: 456),
                location: TokenLocation(startIndex: 6, length: 3)
            ),
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
                location: TokenLocation(startIndex: 0, length: 2)
            ),
            Token(
                category: .asterisk,
                location: TokenLocation(startIndex: 2, length: 1)
            ),
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

    func testLookAhead() {
        let lexer = Lexer(expression: "*")
        let expectedToken1 = Token(
            category: .asterisk,
            location: TokenLocation(startIndex: 0, length: 1)
        )
        let expectedToken2 = Token(
            category: .eof,
            location: TokenLocation(startIndex: 1, length: 1)
        )

        guard try! lexer.lookAhead() == expectedToken1 else {
            XCTFail("token type error.")
            return
        }
        guard try! lexer.nextToken() == expectedToken1 else {
            XCTFail("token type error.")
            return
        }
        guard try! lexer.lookAhead() == expectedToken2 else {
            XCTFail("token type error.")
            return
        }
        guard try! lexer.nextToken() == expectedToken2 else {
            XCTFail("token type error.")
            return
        }
    }

    static var allTests = [
        ("testInvalidCharacter", testInvalidCharacter),
        ("testIdentifierToken", testIdentifierToken),
        ("testNumberToken", testNumberToken),
        ("testAsteriskToken", testAsteriskToken),
        ("testLookAhead", testLookAhead),
    ]
}
