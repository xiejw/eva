protocol ASTExpression {}

class ASTCronExpression: ASTExpression {
    private let minute: ASTField
    private let hour: ASTField
    private let day: ASTField
    private let month: ASTField
    private let dayOfWeek: ASTField

    init(minute: ASTField, hour: ASTField, day: ASTField, month: ASTField, dayOfWeek: ASTField) {
        self.minute = minute
        self.hour = hour
        self.day = day
        self.month = month
        self.dayOfWeek = dayOfWeek
    }

    func codegen() -> CronExpression {
        return CronExpression(
            minute: minute.field,
            hour: hour.field,
            day: day.field,
            month: month.field,
            dayOfWeek: dayOfWeek.field
        )
    }
}

protocol ASTField {
    var field: Field { get }
}

class ASTSingleValueField: ASTField {
    let value: UInt

    init(value: UInt) {
        self.value = value
    }

    var field: Field {
        return .singleValue(Int(value))
    }
}

class ASTAnyValueField: ASTField {
    var field: Field {
        return .any
    }
}
