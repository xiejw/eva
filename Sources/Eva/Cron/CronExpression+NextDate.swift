import Foundation

/// Algorithm to find next date matching the CronExpression.
///
/// Step 1: Go to next minute and reset the second field.
/// Step 2 (Entry:) : Search minute until match.
/// Step 3: Search hour until match. If hour changed, reset lower components
///         (minute in this case) and jump to `Entry`.
/// Step 4: Search day until match. If day changed, reset lower components
///         (hour and minute in this case) and jump to `Entry`.
/// Step 5-6: Search month and year. Same rule as step 3 and 4.
extension CronExpression {

  /// Returns the next date matching `CronExpression`.
  public func nextDate(from: Date = Date()) -> Date? {
    // Add one minute to start the searching.
    guard var candidate = calendar.date(
      byAdding: .minute,
      value: 1,
      to: from
    ) else {
      fatalError("Date \(from) cannot go forward anymore.")
    }

    // Reset second.
    candidate = rewind(candidate, upTo: .minute)

    mainLoop: while true {
      for component in Array<Calendar.Component>(
        [.minute, .hour, .day, .month, .year]
      ) {
        let (newCandidate, changed) = searchNextMatching(
          for: component, from: candidate
        )
        if changed {
          candidate = newCandidate
          continue mainLoop
        }
      }
      break
    }
    return candidate
  }

  /// Searches the next matching value. This algorithrm only scans values on
  /// single component. if the component is not same as the initial value, all
  /// lower components will be rewound.
  private func searchNextMatching(
    for component: Calendar.Component, from startPoint: Date
  ) -> (newCandidate: Date, changed: Bool)
  {

    let testField = field(for: component)

    let initialValue = calendar.component(component, from: startPoint)
    if testField ~= initialValue {
      return (startPoint, false)
    }

    var candidate = startPoint
    candidate = rewind(candidate, upTo: component)

    while true {
      guard let newCandidate = calendar.date(
        byAdding: component,
        value: 1,
        to: candidate
      ) else {
        fatalError("Cannot search forward anymore: \(candidate).")
      }
      candidate = newCandidate
      let newValue = calendar.component(component, from: candidate)
      if testField ~= newValue {
        return (candidate, true)
      }
    }
  }

  /// Rewinds all components in the candidate up to `upTo`. For example, if
  /// `upTo` is `.day`, then `.hour`, `.minute`, and `.second` are rewinded to
  /// zero.
  private func rewind(_ candidate: Date, upTo: Calendar.Component) -> Date {
    // calendar.dateComponents will crash. So, use manual way.
    var components = DateComponents()
    components.timeZone = calendar.timeZone
    components.year = calendar.component(.year, from: candidate)
    components.month = calendar.component(.month, from: candidate)
    components.day = calendar.component(.day, from: candidate)
    components.hour = calendar.component(.hour, from: candidate)
    components.minute = calendar.component(.minute, from: candidate)
    components.second = calendar.component(.second, from: candidate)

    switch upTo {
    case .year:
      components.month = 1
      fallthrough
    case .month:
      components.day = 1
      fallthrough
    case .day:
      components.hour = 0
      fallthrough
    case .hour:
      components.minute = 0
      fallthrough
    case .minute:
      components.second = 0
    default:
      fatalError("Component \(upTo) is not supported.")
    }
    guard let newDate = calendar.date(from: components) else {
      fatalError("Component \(components) is not supported.")
    }
    return newDate
  }

  /// Helper method to get the corresponding field according to component.
  private func field(for component: Calendar.Component) -> Field {
    switch component {
    case .year: return self.year
    case .month: return self.month
    case .day: return self.day
    case .hour: return self.hour
    case .minute: return self.minute
    default:
      fatalError("Component \(component) is not supported.")
    }
  }
}
