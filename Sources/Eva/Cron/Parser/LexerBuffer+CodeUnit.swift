/// Adds indicator for LexerBuffer.CodeUnit.
extension LexerBuffer.CodeUnit {

  typealias ASCII = LexerBuffer.ASCII

  var isSpace: Bool {
    get {
      return ASCII.space.rawValue == self
    }
  }

  var isDigit: Bool {
    get {
      return (ASCII.digit0.rawValue...ASCII.digit9.rawValue) ~= self
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

  var isAsterisk: Bool {
    get {
      return ASCII.asterisk.rawValue == self
    }
  }
}
