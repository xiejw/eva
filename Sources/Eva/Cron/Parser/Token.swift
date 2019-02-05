enum Token: Equatable {
  case eof
  case number
  case identifier(identifier: String)
  case whiteSpaces(length: Int)
}
