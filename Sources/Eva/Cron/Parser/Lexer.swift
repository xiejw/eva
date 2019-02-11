import Foundation

enum LexerError: Error {
  case unexpectedCharacter(character: Character)
}

/// Generates tokens for cron expression.
class Lexer {

  typealias CodeUnit = LexerBuffer.CodeUnit

  private let expression: String
  private var buffer: LexerBuffer
  private var lookAheadToken: Token?

  init(expression: String) {
    self.expression = expression
    self.buffer =  LexerBuffer(expression: expression)
  }

  /// Returns the next `Token`.
  func nextToken() throws -> Token {
    // If hookAhook is cached, use it.
    if let result = lookAheadToken {
      // Reset lookAhead
      lookAheadToken = nil
      return result
    }
    // Gets the next token for real.
    return try readTokenFromBuffer()
  }

  /// Looks ahead of the `Token`.
  func lookAhead() throws -> Token {
    if let result = lookAheadToken {
      return result
    }
    let token = try readTokenFromBuffer()
    lookAheadToken = token
    return token
  }

  private func readTokenFromBuffer() throws -> Token {
    let character = buffer.nextCharacter()
    switch character {
    case .eof(let index):
      let location = TokenLocation(startIndex: index, length: 1)
      return Token(category: .eof, location: location)
    case let .character(value, index):
      if value.isAsterisk {
        let location = TokenLocation(startIndex: index, length: 1)
        return Token(category: .asterisk, location: location)
      }
      if value.isSpace {
        buffer.rollback()
        return nextTokenForWhiteSpaces(startAt: index)
      }
      if value.isLetter {
        buffer.rollback()
        return nextTokenForIdentifider(startAt: index)
      }
      if value.isDigit {
        buffer.rollback()
        return nextTokenForNumber(startAt: index)
      }
      throw LexerError.unexpectedCharacter(character: character)
    }
  }
}

// Helper methods to generate tokens for Lexer.
fileprivate extension Lexer {

  func nextTokenForWhiteSpaces(startAt startIndex: Int) -> Token {
    var length = 0
    while true {
      let character = buffer.nextCharacter()
      guard case let .character(value, _) = character else {
        break
      }
      guard value.isSpace else {
        break
      }
      length += 1
    }
    buffer.rollback()
    let location = TokenLocation(startIndex: startIndex, length: length)
    return Token(category: .whiteSpaces(length: length), location: location)
  }

  func nextTokenForIdentifider(startAt startIndex: Int) -> Token {
    var chars: [CodeUnit] = []
    while true {
      let character = buffer.nextCharacter()
      guard case let .character(value, _) = character else {
        break
      }
      guard value.isLetter else {
        break
      }
      chars.append(value)
    }
    buffer.rollback()
    guard let identifier = String(bytes: chars, encoding: .utf8) else {
      fatalError("Failed to convert \(chars) to string.")
    }
    let location = TokenLocation(startIndex: startIndex, length: chars.count)
    return Token(category: .identifier(identifier: identifier), location: location)
  }

  func nextTokenForNumber(startAt startIndex: Int) -> Token {
    var length = 0
    var numericalValue: UInt = 0
    while true {
      let character = buffer.nextCharacter()
      guard case let .character(value, _) = character else {
        break
      }
      guard value.isDigit else {
        break
      }
      numericalValue *= 10
      numericalValue += UInt(value - LexerBuffer.ASCII.digit0.rawValue)
      length += 1
    }
    buffer.rollback()
    let location = TokenLocation(startIndex: startIndex, length: length)
    return Token(category: .number(value: numericalValue), location: location)
  }
}

