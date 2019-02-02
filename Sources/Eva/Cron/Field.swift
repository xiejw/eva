enum Field {
  // `*` in cron expression.
  case any
  case singleValue(Int)
}

extension Field {
  static func ~=(_ lhs: Field, _ rhs: Int) -> Bool {
    switch lhs {
    case .any:
      return true
    case let .singleValue(value):
      return value == rhs
    }
  }
}

