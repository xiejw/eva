add_library(CronExpression
  Expression.cpp
  Expression.h
  Field.h
)

set(CRON_LIBS ${CRON_LIBS} CronExpression PARENT_SCOPE)

add_library(TestCronExpression OBJECT
  ExpressionTest.cpp
)

target_include_directories(TestCronExpression PUBLIC ${GTEST_INCLUDES})
set(TEST_CRON_LIBS ${TEST_CRON_LIBS} TestCronExpression PARENT_SCOPE)
