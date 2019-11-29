add_library(Crypto
  Sha256.cpp
  Sha256.h
)

set(CRYPTO_LIBS ${CRYPTO_LIBS} Crypto PARENT_SCOPE)

add_library(TestCrypto OBJECT
  Sha256Test.cpp
)

target_include_directories(TestCrypto PUBLIC ${GTEST_INCLUDES})
set(TEST_CRYPTO_LIBS ${TEST_CRYPTO_LIBS} TestCrypto PARENT_SCOPE)
