enum Field: Equatable {
    // `*` in cron expression.
    case any
    case singleValue(Int)
}

extension Field {
    /// A pattern matching of the field.
    static func ~= (_ lhs: Field, _ rhs: Int) -> Bool {
        switch lhs {
        case .any:
            return true
        case let .singleValue(value):
            return value == rhs
        }
    }
}
