import Foundation

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
  func nextToken() -> Token {
    while true {
      guard let character = buffer.nextCharacter() else {
        return .eof
      }

      // if character.isSpace {
      //   continue
      // }

      if character.isLetter {
        buffer.rollback()
        return consumeCharactersForIdentifier()
      }
    }

  }
}

// Helper methods to generate tokens for Lexer.
fileprivate extension Lexer {

  func consumeCharactersForIdentifier() -> Token {
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
}

