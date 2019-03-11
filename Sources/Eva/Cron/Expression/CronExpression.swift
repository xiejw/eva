import Foundation

// FIXME: remove all public access.
public struct CronExpression {
    let minute: Field
    let hour: Field
    let day: Field
    let month: Field
    let dayOfWeek: Field
    let year: Field
    let maxNumYearsToConsider: Int

    // Fixed calendar to be gregorian due to day of week calculation.
    let calendar = Calendar(identifier: Calendar.Identifier.gregorian)

    enum DayOfWeek: Int {
        case sunday = 1
        case monday = 2
        case tuesday = 3
        case wednesday = 4
        case thusday = 5
        case friday = 6
        case saturday = 7
    }

    init(
        minute: Field = .any,
        hour: Field = .any,
        day: Field = .any,
        month: Field = .any,
        dayOfWeek: Field = .any,
        year: Field = .any,
        maxNumYearsToConsider: Int = 5
    ) {
        self.minute = minute
        self.hour = hour
        self.day = day
        self.month = month
        self.dayOfWeek = dayOfWeek
        self.year = year
        self.maxNumYearsToConsider = maxNumYearsToConsider
    }
}

// Adds pattern matching.
extension CronExpression {
    static func ~= (pattern: CronExpression, date: Date) -> Bool {
        let minute = pattern.calendar.component(.minute, from: date)
        guard pattern.minute ~= minute else {
            return false
        }

        let hour = pattern.calendar.component(.hour, from: date)
        guard pattern.hour ~= hour else {
            return false
        }

        let day = pattern.calendar.component(.day, from: date)
        guard pattern.day ~= day else {
            return false
        }

        let dayOfWeek = pattern.calendar.component(.weekday, from: date)
        guard pattern.dayOfWeek ~= dayOfWeek else {
            return false
        }

        let month = pattern.calendar.component(.month, from: date)
        guard pattern.month ~= month else {
            return false
        }

        let year = pattern.calendar.component(.year, from: date)
        guard pattern.year ~= year else {
            return false
        }
        return true
    }
}
