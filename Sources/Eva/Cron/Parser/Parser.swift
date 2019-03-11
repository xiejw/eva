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

    // TopLevelExpress := Space? CronExpression Space?
    func parseTopLevelExpression() throws -> ASTCronExpression {
        if case .whiteSpaces = try lexer.lookAhead().category { try parseEmptySpaces() }
        let expr = try parseCronExpression()
        if case .whiteSpaces = try lexer.lookAhead().category { try parseEmptySpaces() }

        let finalToken = try lexer.lookAhead()
        guard case .eof = finalToken.category else { reportError(expected: "eof", got: finalToken) }
        return expr
    }

    private func parseEmptySpaces() throws {
        let token = try lexer.nextToken()
        guard case .whiteSpaces = token.category else {
            throw ParserError.unexpectedToken(expected: "Spaces", got: token)
        }
    }

    private func parseCronExpression() throws -> ASTCronExpression {
        let second = try parseSecondField()
        try parseEmptySpaces()
        let minute = try parseMinuteField()
        let expr = ASTCronExpression(second: second, minute: minute)
        return expr
    }

    private func parseSecondField() throws -> ASTField {
        return ASTSingleValueField(value: 0)
    }

    private func parseMinuteField() throws -> ASTField {
        return ASTSingleValueField(value: 0)
    }

    private func reportError(expected: String, got token: Token) -> Never {
        fatalError("Unexpected token: expected \(expected), got \(token).")
    }

    private func reportError(_ character: Character) -> Never {
        fatalError("Unexpected character: \(character).")
    }
}
