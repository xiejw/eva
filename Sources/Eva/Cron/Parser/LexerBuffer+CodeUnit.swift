/// Adds indicator for LexerBuffer.CodeUnit.
extension LexerBuffer.CodeUnit {
    typealias ASCII = LexerBuffer.ASCII

    var isSpace: Bool {
        return ASCII.space.rawValue == self
    }

    var isDigit: Bool {
        return (ASCII.digit0.rawValue ... ASCII.digit9.rawValue) ~= self
    }

    var isLetter: Bool {
        return isLowerCaseLetter || isUpperCaseLetter
    }

    var isLowerCaseLetter: Bool {
        return (ASCII.a.rawValue ... ASCII.z.rawValue) ~= self
    }

    var isUpperCaseLetter: Bool {
        return (ASCII.A.rawValue ... ASCII.Z.rawValue) ~= self
    }

    var isAsterisk: Bool {
        return ASCII.asterisk.rawValue == self
    }
}
