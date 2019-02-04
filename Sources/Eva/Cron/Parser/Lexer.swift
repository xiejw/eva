/// Generates tokens for cron expression.
class Lexer {

  typealias Character = LexerBuffer.Character

  private let expression: String
  private var buffer: LexerBuffer

  init(expression: String) {
    self.expression = expression
    self.buffer =  LexerBuffer(expression: expression)
  }
}

