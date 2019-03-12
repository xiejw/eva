enum ParserError: Error {
    case unexpectedToken(expected: String, got: Token)
}

class Parser {
    private let expression: String
    private let lexer: Lexer

    init(expression: String) {
        self.expression = expression
        lexer = Lexer(expression: expression)
    }

    /// Parses the `expression` and returns the valid `CronExpression`.
    func parse() -> CronExpression {
        do {
            return try parseTopLevelExpression().codegen()
        } catch let LexerError.unexpectedCharacter(character) {
            reportError(character)
        } catch let ParserError.unexpectedToken(expected, got) {
            reportError(expected: expected, got: got)
        } catch {
            fatalError("Unexpectec error: \(error).")
        }
    }
}

extension Parser {
    // TopLevelExpress := Space? CronExpression Space?
    func parseTopLevelExpression() throws -> ASTCronExpression {
        if case .whiteSpaces = try lexer.lookAhead().category { try parseEmptySpaces() }
        let expr = try parseCronExpression()
        if case .whiteSpaces = try lexer.lookAhead().category { try parseEmptySpaces() }

        let finalToken = try lexer.lookAhead()
        guard case .eof = finalToken.category else { reportError(expected: "eof", got: finalToken) }
        return expr
    }
}

extension Parser {
    private func parseEmptySpaces() throws {
        let token = try lexer.nextToken()
        guard case .whiteSpaces = token.category else {
            throw ParserError.unexpectedToken(expected: "Spaces", got: token)
        }
    }
}

extension Parser {
    private func parseCronExpression() throws -> ASTCronExpression {
        let minute = try parseMinuteField()
        try parseEmptySpaces()
        let hour = try parseHourField()
        try parseEmptySpaces()
        let day = try parseDayField()

        let expr = ASTCronExpression(minute: minute, hour: hour, day: day)
        return expr
    }

    private func parseMinuteField() throws -> ASTField {
        return try parseSingleValueOrAny(for: "minute")
    }

    private func parseHourField() throws -> ASTField {
        return try parseSingleValueOrAny(for: "hour")
    }

    private func parseDayField() throws -> ASTField {
        return try parseSingleValueOrAny(for: "day")
    }

    private func parseSingleValueOrAny(for fieldName: String) throws -> ASTField {
        let token = try lexer.nextToken()
        switch token.category {
        case .asterisk: return ASTAnyValueField()
        case let .number(value): return ASTSingleValueField(value: value)
        default:
            throw ParserError.unexpectedToken(
                expected: "A valid expression for \(fieldName).",
                got: token
            )
        }
    }
}

extension Parser {
    private func reportError(expected: String, got token: Token) -> Never {
        fatalError("Unexpected token: expected \(expected), got \(token).")
    }

    private func reportError(_ character: Character) -> Never {
        fatalError("Unexpected character: \(character).")
    }
}
