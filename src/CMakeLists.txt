add_library(EVACron
  Expression.cpp
  Expression.h
  Field.h
)

set(EVA_CRON_LIBS ${EVA_CRON_LIBS} EVACron PARENT_SCOPE)

add_library(TestEVACron OBJECT
  ExpressionTest.cpp
)

target_include_directories(TestEVACron PUBLIC ${GTEST_INCLUDES})
set(TEST_EVA_CRON_LIBS ${TEST_EVA_CRON_LIBS} TestEVACron PARENT_SCOPE)
