/// Defines the ASCII table.
extension LexerBuffer {
  enum ASCII: CodeUnit {
    case space = 32
    case asterisk = 42

    case digit0 = 48
    case digit9 = 57

    case A = 65
    case Z = 90

    case a = 97
    case z = 122
  }
}
