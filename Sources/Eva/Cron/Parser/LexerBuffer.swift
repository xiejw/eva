struct LexerBuffer {
  typealias Character = UInt8

  private let exprBuffer: [Character]
  private var currentIndex = 0

  init(expression: String) {
    self.exprBuffer = Array(expression.utf8).map { Character($0) }
  }

  mutating func nextCharacter() -> Character? {
    guard currentIndex < exprBuffer.count else {
      return nil
    }
    let char = exprBuffer[currentIndex]
    currentIndex += 1
    return char
  }

  mutating func rollback() {
    precondition(currentIndex > 0)
    currentIndex -= 1
  }
}
