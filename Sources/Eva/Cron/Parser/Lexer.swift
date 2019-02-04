/// Generates tokens for cron expression.
class Lexer {

  typealias Character = Int

  private let expression: String
  private let exprBuffer: [Character]
  private var currentIndex = 0

  init(expression: String) {
    self.expression = expression
    self.exprBuffer = Array(expression.utf8).map { Character($0) }
  }

  func nextCharacter() -> Character? {
    guard currentIndex < exprBuffer.count else {
      return nil
    }
    let char = exprBuffer[currentIndex]
    currentIndex += 1
    return char
  }

  func rollback() {
    precondition(currentIndex > 0)
    currentIndex -= 1
  }
}

extension Lexer {
  enum ASCII: Int {
    case a = 97
  }
}
