/// Adds indicator for LexerBuffer.Character.
extension LexerBuffer.Character {

  typealias ASCII = LexerBuffer.ASCII

  var isSpace: Bool {
    get {
      return ASCII.space.rawValue == self
    }
  }

  var isLetter: Bool {
    get {
      return isLowerCaseLetter || isUpperCaseLetter
    }
  }

  var isLowerCaseLetter: Bool {
    get {
      return (ASCII.a.rawValue...ASCII.z.rawValue) ~= self
    }
  }

  var isUpperCaseLetter: Bool {
    get {
      return (ASCII.A.rawValue...ASCII.Z.rawValue) ~= self
    }
  }
}
