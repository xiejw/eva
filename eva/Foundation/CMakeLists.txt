add_library(EvaFoundation
  Macros.h
  Status.h
  Status.cpp
  StatusOr.h
)

set(EVA_FOUNDATION_LIBS ${EVA_FOUNDATION_LIBS} Foundation PARENT_SCOPE)

# add_library(TestFoundation OBJECT
#   StatusTest.cpp
#   StatusOrTest.cpp
# )
#
# target_include_directories(TestFoundation PUBLIC ${GTEST_INCLUDES})
# set(TEST_MLVM_LIBS ${TEST_MLVM_LIBS} TestFoundation PARENT_SCOPE)
