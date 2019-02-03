import Foundation

struct CronExpression {
  let minute: Field
  let hour: Field
  let day: Field
  let month: Field
  let year: Field

  init(
    minute: Field = .any,
    hour: Field = .any,
    day: Field = .any,
    month: Field = .any,
    year: Field = .any
  ) {
    self.minute = minute
    self.hour = hour
    self.day = day
    self.month = month
    self.year = year
  }
}

// Adds pattern matching.
extension CronExpression {

  static func ~=(pattern: CronExpression, date: Date) -> Bool {
    let calendar = Calendar.current

    let minute = calendar.component(.minute, from: date)
    guard pattern.minute ~= minute else {
      return false
    }

    let hour = calendar.component(.hour, from: date)
    guard pattern.hour ~= hour else {
      return false
    }

    let day = calendar.component(.day, from: date)
    guard pattern.day ~= day else {
      return false
    }

    let month = calendar.component(.month, from: date)
    guard pattern.month ~= month else {
      return false
    }

    let year = calendar.component(.year, from: date)
    guard pattern.year ~= year else {
      return false
    }
    return true
  }
}
