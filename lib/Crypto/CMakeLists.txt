add_library(Crypto
  Sha256.cpp
  Sha256.h
)

set(CRYPTO_LIBS ${CRYPTO_LIBS} Crypto PARENT_SCOPE)
