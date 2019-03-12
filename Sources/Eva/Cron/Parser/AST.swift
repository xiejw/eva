protocol ASTExpression {}

class ASTCronExpression: ASTExpression {
    private var minute: ASTField

    init(minute: ASTField) {
        self.minute = minute
    }

    func codegen() -> CronExpression {
        return CronExpression(
            minute: minute.field
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
