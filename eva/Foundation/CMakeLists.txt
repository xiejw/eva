add_library(EVAFoundation
  Macros.h
  Status.h
  Status.cpp
  StatusOr.h
)

set(EVA_FOUNDATION_LIBS ${EVA_FOUNDATION_LIBS} EVAFoundation PARENT_SCOPE)

add_library(TestEVAFoundation OBJECT
  StatusTest.cpp
  StatusOrTest.cpp
)

target_include_directories(TestEVAFoundation PUBLIC ${GTEST_INCLUDES})
set(TEST_EVA_FOUNDATION_LIBS ${TEST_EVA_FOUNDATION_LIBS} TestEVAFoundation PARENT_SCOPE)
