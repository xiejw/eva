add_library(Expression
  Expression.cpp
  Expression.h
  Field.h
)

set(CRON_LIBS ${CRON_LIBS} Expression PARENT_SCOPE)

add_library(TestExpression OBJECT
  ExpressionTest.cpp
)

target_include_directories(TestExpression PUBLIC ${GTEST_INCLUDES})
set(TEST_CRON_LIBS ${TEST_CRON_LIBS} TestExpression PARENT_SCOPE)
