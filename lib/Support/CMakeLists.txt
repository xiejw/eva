add_library(Support
  Error.cpp
  Error.h
)

set(SUPPORT_LIBS ${SUPPORT_LIBS} Support PARENT_SCOPE)

add_library(TestSupport OBJECT
  ErrorTest.cpp
)

target_include_directories(TestSupport PUBLIC ${GTEST_INCLUDES})
set(TEST_SUPPORT_LIBS ${TEST_SUPPORT_LIBS} TestSupport PARENT_SCOPE)
