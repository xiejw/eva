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

    if case .whiteSpaces(_) = lexer.lookAhead().category {
      // Eat it.
      _ = lexer.nextToken()
    }

    let expr = ASTCronExpression()

    if case .whiteSpaces(_) = lexer.lookAhead().category {
      // Eat it.
      _ = lexer.nextToken()
    }

    let finalToken = lexer.lookAhead()
    guard case .eof = finalToken.category {

  }
}
