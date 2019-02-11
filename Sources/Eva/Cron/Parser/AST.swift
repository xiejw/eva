protocol ASTExpression {
}

class ASTCronExpression: ASTExpression {

  func codegen() -> CronExpression {
    return CronExpression()
  }
}

class ASTEmptySpacces: ASTExpression {
}

class ASTField: ASTExpression {
}

class ASTSingleValueField: ASTField {
  private let value: Int

  init(value: Int) {
    self.value = value
  }
}

