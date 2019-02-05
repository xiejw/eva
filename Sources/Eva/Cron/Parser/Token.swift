enum Token: Equatable {

  /// End of file (file means expression in this context).
  case eof

  /// Only positive decimal number are allowed.
  case number(value: UInt)

  /// Identifier is ([a-z][A-Z])+. No, underscore, digits, etc.
  case identifier(identifier: String)

  /// White spaces cannot be ignored in Cron expresion.
  case whiteSpaces(length: Int)
}
