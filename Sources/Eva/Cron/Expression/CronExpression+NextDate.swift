import Foundation

enum InvalidExpression: Error {
    case reachMaxNumYearsToSearch
}

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
    public func nextDate(from: Date) -> Date? {
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
        let startPoint = candidate

        mainLoop: while true {
            for component in [Calendar.Component](
                [.minute, .hour, .day, .weekday, .month, .year]
            ) {
                do {
                    let (newCandidate, changed) = try searchNextMatching(
                        for: component, from: candidate, startPoint: startPoint
                    )

                    if changed {
                        candidate = newCandidate
                        continue mainLoop
                    }
                } catch InvalidExpression.reachMaxNumYearsToSearch {
                    // TODO: Use better stderr logging.
                    print("Reach maximum number of years to search.")
                    return nil
                } catch {
                    // TODO: Use better stderr logging.
                    print("Unknown error.")
                    return nil
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
        for component: Calendar.Component,
        from currentCandidate: Date,
        startPoint: Date
    ) throws -> (newCandidate: Date, changed: Bool) {
        let testField = field(for: component)

        let initialValue = calendar.component(component, from: currentCandidate)
        if testField ~= initialValue {
            return (currentCandidate, false)
        }

        var candidate = currentCandidate
        candidate = rewind(candidate, upTo: component)

        while true {
            // For .weekday (dayOfWeek), we increase .day component.
            let incrementalComponent = component != .weekday ? component : .day
            guard let newCandidate = calendar.date(
                byAdding: incrementalComponent,
                value: 1,
                to: candidate
            ) else {
                fatalError("Cannot search forward anymore: \(candidate).")
            }

            try validateOnChange(
                changedComponent: component,
                initialValue: startPoint,
                beforeChangeValue: candidate,
                afterChangeValue: newCandidate
            )

            let newValue = calendar.component(component, from: newCandidate)
            candidate = newCandidate

            if testField ~= newValue {
                return (candidate, true)
            }
        }
    }

    /// Validates whether the `newValue` is acceptable (the year component should
    /// not change too much.)
    private func validateOnChange(
        changedComponent component: Calendar.Component,
        initialValue: Date,
        beforeChangeValue _: Date,
        afterChangeValue newValue: Date
    ) throws {
        // This algorithm does a fast check about whether the number of years have
        // been increased more than maxNumYearsToConsider. We ignore minute and hour
        // as they do not consider to year change. We consider day, month,
        // dayOfWeek, and year as some combinations, say (day, month, dayOfWeek),
        // lead to year value change.
        guard component != .minute, component != .hour else {
            return
        }
        let initialYearValue = calendar.component(.year, from: initialValue)
        let newYearValue = calendar.component(.year, from: newValue)
        guard newYearValue - initialYearValue < maxNumYearsToConsider else {
            throw InvalidExpression.reachMaxNumYearsToSearch
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
        case .weekday, .day:
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
        case .year: return year
        case .month: return month
        case .weekday: return dayOfWeek
        case .day: return day
        case .hour: return hour
        case .minute: return minute
        default:
            fatalError("Component \(component) is not supported.")
        }
    }
}
