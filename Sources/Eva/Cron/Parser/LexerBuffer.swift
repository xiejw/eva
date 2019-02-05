struct LexerBuffer {
  typealias Character = UInt8

  private let exprBuffer: [Character]
  private var currentIndex = 0

  init(expression: String) {
    self.exprBuffer = Array(expression.utf8).map { Character($0) }
  }

  /// Returns the next character, or `nil` if eof.
  ///
  /// Eof should not be fetched twice consecutively,
  mutating func nextCharacter() -> Character? {
    guard currentIndex <= exprBuffer.count else {
      fatalError("Eof is reached twice consecutively, which is not allowed.")
    }
    guard currentIndex < exprBuffer.count else {
      // Increase index also so the rollback can work.
      currentIndex += 1
      return nil
    }
    let char = exprBuffer[currentIndex]
    currentIndex += 1
    return char
  }


  /// Rollbacks the buffer to previous character.
  ///
  /// `rollback` also supports `nextCharacter()` returns `nil`, i.e.,
  ///
  ///     nextCharacter() => nil
  ///     rollback()
  ///     nextCharacter() => nil
  mutating func rollback() {
    precondition(currentIndex > 0)
    currentIndex -= 1
  }
}
