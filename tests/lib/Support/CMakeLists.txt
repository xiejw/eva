add_library(TestSupport OBJECT
  ErrorTest.cpp
)

target_include_directories(TestSupport PUBLIC ${GTEST_INCLUDES})
set(TEST_LIBS ${TEST_LIBS} TestSupport PARENT_SCOPE)
