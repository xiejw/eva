enum  Character {
  case eof(index: Int)
  case character(value: UInt8, index: Int)
}

struct LexerBuffer {
  typealias CodeUnit = UInt8

  private let exprBuffer: [CodeUnit]
  private var index = 0

  init(expression: String) {
    self.exprBuffer = Array(expression.utf8).map { CodeUnit($0) }
  }

  /// Returns the next character, or `nil` if eof.
  ///
  /// Eof should not be fetched twice consecutively,
  mutating func nextCharacter() -> Character {
    let currentIndex = index
    guard index <= exprBuffer.count else {
      fatalError("Eof is reached twice consecutively, which is not allowed.")
    }
    guard index < exprBuffer.count else {
      // Increase index also so the rollback can work.
      index += 1
      return .eof(index: currentIndex)
    }
    let char = exprBuffer[index]
    index += 1
    return .character(value: char, index: currentIndex)
  }


  /// Rollbacks the buffer to previous character.
  ///
  /// `rollback` also supports `nextCharacter()` returns `nil`, i.e.,
  ///
  ///     nextCharacter() => nil
  ///     rollback()
  ///     nextCharacter() => nil
  mutating func rollback() {
    precondition(index > 0)
    index -= 1
  }
}
