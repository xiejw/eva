import Foundation

enum LexerError: Error {
  case unexpectedCharacter(character: LexerBuffer.Character)
}

/// Generates tokens for cron expression.
class Lexer {

  typealias Character = LexerBuffer.Character

  private let expression: String
  private var buffer: LexerBuffer

  init(expression: String) {
    self.expression = expression
    self.buffer =  LexerBuffer(expression: expression)
  }

  // TODO: Adds good error message.
  func nextToken() throws -> Token {
    while true {
      guard let character = buffer.nextCharacter() else {
        return .eof
      }

      if character.isSpace {
        buffer.rollback()
        return nextTokenForWhiteSpaces()
      }

      if character.isLetter {
        buffer.rollback()
        return nextTokenForIdentifider()
      }

      if character.isDigit {
        buffer.rollback()
        return nextTokenForNumber()
      }

      if character.isAsterisk { return .asterisk }

      throw LexerError.unexpectedCharacter(character: character)
    }

  }
}

// Helper methods to generate tokens for Lexer.
fileprivate extension Lexer {

  func nextTokenForWhiteSpaces() -> Token {
    var length = 0
    while true {
      guard let character = buffer.nextCharacter() else {
        break
      }
      guard character.isSpace else {
        break
      }
      length += 1
    }
    buffer.rollback()
    return .whiteSpaces(length: length)
  }

  func nextTokenForIdentifider() -> Token {
    var chars: [Character] = []
    while true {
      guard let character = buffer.nextCharacter() else {
        break
      }
      guard character.isLetter else {
        break
      }
      chars.append(character)
    }
    buffer.rollback()
    guard let identifier = String(bytes: chars, encoding: .utf8) else {
      fatalError("Failed to convert \(chars) to string.")
    }
    return .identifier(identifier: identifier)
  }

  func nextTokenForNumber() -> Token {
    var value: UInt = 0
    while true {
      guard let character = buffer.nextCharacter() else {
        break
      }
      guard character.isDigit else {
        break
      }
      value = 10 * value + UInt(character - LexerBuffer.ASCII.digit0.rawValue)
    }
    buffer.rollback()
    return .number(value: value)
  }
}

