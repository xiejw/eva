add_library(TestExpression OBJECT
  ExpressionTest.cpp
)

target_include_directories(TestExpression PUBLIC ${GTEST_INCLUDES})
set(TEST_LIBS ${TEST_LIBS} TestExpression PARENT_SCOPE)
