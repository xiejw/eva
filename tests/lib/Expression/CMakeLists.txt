add_library(TestExpression OBJECT
  ExpressionTest.cpp
)

target_include_directories(TestExpression PUBLIC ${GTEST_INCLUDES})
