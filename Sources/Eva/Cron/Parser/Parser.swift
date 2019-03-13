enum ParserError: Error {
    case unexpectedToken(expected: String, got: Token)
    case invalidValue(message: String)
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
        try parseEmptySpaces()
        let month = try parseMonthField()
        try parseEmptySpaces()
        let dayOfWeek = try parseDayOfWeekField()

        return ASTCronExpression(
            minute: minute,
            hour: hour,
            day: day,
            month: month,
            dayOfWeek: dayOfWeek
        )
    }

    private func parseMinuteField() throws -> ASTField {
        // TODO(xiejw): Validate the value.
        return try parseSingleValueOrAny(for: "minute")
    }

    private func parseHourField() throws -> ASTField {
        // TODO(xiejw): Validate the value.
        return try parseSingleValueOrAny(for: "hour")
    }

    private func parseDayField() throws -> ASTField {
        // TODO(xiejw): Validate the value.
        return try parseSingleValueOrAny(for: "day")
    }

    private func parseMonthField() throws -> ASTField {
        // TODO(xiejw): Validate the value.
        return try parseSingleValueOrAny(for: "month")
    }

    private func parseDayOfWeekField() throws -> ASTField {
        let field = try parseSingleValueOrAny(for: "day of week")
        if let singleValue = field as? ASTSingleValueField {
            // The day of week in cron expression starts with 0 (sunday) and ends with 6 (saturday).
            // The special value 7 also means sunday.
            switch singleValue.value {
            case 0, 7:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.sunday.rawValue))
            case 1:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.monday.rawValue))
            case 2:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.tuesday.rawValue))
            case 3:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.wednesday.rawValue))
            case 4:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.thursday.rawValue))
            case 5:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.friday.rawValue))
            case 6:
                return ASTSingleValueField(value: UInt(CronExpression.DayOfWeek.saturday.rawValue))
            default:
                throw ParserError.invalidValue(
                    message: "invalid value (\(singleValue.value)) for day of week."
                )
            }
        }

        if let anyValue = field as? ASTAnyValueField {
            return anyValue
        }

        fatalError("Unexpected type of \(field)")
    }

    private func parseSingleValueOrAny(for fieldName: String) throws -> ASTField {
        let token = try lexer.nextToken()
        switch token.category {
        case .asterisk: return ASTAnyValueField()
        case let .number(value): return ASTSingleValueField(value: value)
        default:
            throw ParserError.unexpectedToken(
                expected: "A valid expression for \(fieldName) is expected.",
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
