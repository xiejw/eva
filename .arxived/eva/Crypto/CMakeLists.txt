add_library(EVACrypto
  Sha256.cpp
  Sha256.h
)

set(EVA_CRYPTO_LIBS ${EVA_CRYPTO_LIBS} EVACrypto PARENT_SCOPE)

add_library(TestEVACrypto OBJECT
  Sha256Test.cpp
)

target_include_directories(TestEVACrypto PUBLIC ${GTEST_INCLUDES})
set(TEST_EVA_CRYPTO_LIBS ${TEST_EVA_CRYPTO_LIBS} TestEVACrypto PARENT_SCOPE)
