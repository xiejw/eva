protocol ASTExpression {}

class ASTCronExpression: ASTExpression {
    private let minute: ASTField
    private let hour: ASTField

    init(minute: ASTField, hour: ASTField) {
        self.minute = minute
        self.hour = hour
    }

    func codegen() -> CronExpression {
        return CronExpression(
            minute: minute.field,
            hour: hour.field
        )
    }
}

protocol ASTField {
    var field: Field { get }
}

class ASTSingleValueField: ASTField {
    private let value: UInt

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
