#include "lib/Crypto/Sha256.h"

#include <string>

#include "gtest/gtest.h"

namespace eva {
namespace crypto {
namespace {

class Sha256Test : public ::testing::Test {};

TEST_F(Sha256Test, CheckEmptyString) {
  std::string digest{sha256("")};
  ASSERT_EQ("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
            digest);
}

TEST_F(Sha256Test, CheckGrape) {
  std::string digest{sha256("grape")};
  ASSERT_EQ("0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496",
            digest);
}

}  // namespace
}  // namespace crypto
}  // namespace eva
