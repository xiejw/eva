protocol ASTExpression {}

class ASTCronExpression: ASTExpression {
    private var second: ASTField
    private var minute: ASTField

    init(second: ASTField, minute: ASTField) {
        self.second = second
        self.minute = minute
    }

    func codegen() -> CronExpression {
        return CronExpression()
    }
}

class ASTField: ASTExpression {}

class ASTSingleValueField: ASTField {
    private let value: Int

    init(value: Int) {
        self.value = value
    }
}
