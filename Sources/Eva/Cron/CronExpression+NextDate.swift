import Foundation

extension CronExpression {

  /// Returns the next date matching cron expression.
  func nextDate(from: Date = Date()) -> Date? {
    let calendar = Calendar.current

    // Add one minute to start the searching.
    guard var candidate = calendar.date(
      byAdding: .minute,
      value: 1,
      to: from
    ) else {
      fatalError("Date \(from) cannot go forward anymore.")
    }

    var wrapped: Bool
    (candidate, wrapped) = searchMinute(from: candidate, calendar: calendar)
    return candidate
  }

  private func searchMinute(from startDate: Date, calendar: Calendar) -> (Date, Bool) {
    let minuteToStart = calendar.component(.minute, from: startDate)
    var wrapped = false
    if self.minute ~= minuteToStart {
      return (startDate, wrapped)
    }

    var candidate = startDate
    while true {
      guard let newCandidate = calendar.date(
        byAdding: .minute,
        value: 1,
        to: candidate
      ) else {
        fatalError("Cannot search forward anymore: \(candidate).")
      }
      candidate = newCandidate
      let minute = calendar.component(.minute, from: candidate)
      if minute < minuteToStart {
        wrapped = true
      }
      if self.minute ~= minute {
        return (candidate, wrapped)
      }
    }
  }
}
