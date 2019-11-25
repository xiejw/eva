add_library(Support
  Error.cpp
  Error.h
)

set(SUPPORT_LIBS ${SUPPORT_LIBS} Support PARENT_SCOPE)

add_library(TestSupport OBJECT
  ErrorTest.cpp
)

target_include_directories(TestSupport PUBLIC ${GTEST_INCLUDES})
set(TEST_LIBS ${TEST_LIBS} TestSupport PARENT_SCOPE)
