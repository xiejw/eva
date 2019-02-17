class Parser {

  private let expression: String
  private let lexer: Lexer

  init(expression: String) {
    self.expression = expression
    self.lexer = Lexer(expression: expression)
  }

  func parse() -> CronExpression {
    return parseCronExpression().codegen()
  }

  private func parseCronExpression() -> ASTCronExpression {
    do {
      if case .whiteSpaces(_) = try lexer.lookAhead().category {
        try parseEmptySpaces()  // Eat it.
      }

      if case .whiteSpaces(_) = try lexer.lookAhead().category {
        try parseEmptySpaces()  // Eat it.
      }

      // let finalToken = lexer.lookAhead()
      // guard case .eof = finalToken.category else {
      // }
      let expr = ASTCronExpression()
      return expr
    } catch let LexerError.unexpectedCharacter(character) {
      reportError(character)
    } catch {
      fatalError("Unexpectec error: \(error).")
    }
  }

  private func parseEmptySpaces() throws { _ = try lexer.nextToken() }

  private func parseSecondField() { }


  private func reportError(_ token: Token) -> Never {
    fatalError("Unexpected token: \(token).")
  }

  private func reportError(_ character: Character) -> Never {
    fatalError("Unexpected character: \(character).")
  }
}
