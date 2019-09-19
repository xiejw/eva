add_library(TestSupport OBJECT
  ErrorTest.cpp
)

target_include_directories(TestSupport PUBLIC ${GTEST_INCLUDE})
