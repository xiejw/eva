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

    return ASTCronExpression()
  }
}
