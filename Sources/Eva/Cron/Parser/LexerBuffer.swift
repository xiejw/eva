/// The character reading from the input stream.
enum  Character: Equatable {
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

  /// Returns the next character.
  ///
  /// `.eof` should not be fetched twice consecutively,
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
  /// `rollback` also supports `nextCharacter()` returns `.eof`, i.e.,
  ///
  ///     nextCharacter() => .eof
  ///     rollback()
  ///     nextCharacter() => .eof
  mutating func rollback() {
    precondition(index > 0, "Rollback should not go beyond the beginning.")
    index -= 1
  }
}
