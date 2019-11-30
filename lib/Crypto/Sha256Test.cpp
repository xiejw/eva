#include "lib/Crypto/Sha256.h"

#include <string>

#include "gtest/gtest.h"

namespace eva {
namespace crypto {
namespace {

class Sha256Test : public ::testing::Test {};

TEST_F(Sha256Test, CheckEmptyString) {
  std::string digest{SHA256::Digest("")};
  ASSERT_EQ("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
            digest);
}

TEST_F(Sha256Test, CheckString) {
  std::string digest{SHA256::Digest("grape")};
  ASSERT_EQ("0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496",
            digest);
}

TEST_F(Sha256Test, CheckLongString) {
  std::string digest{SHA256::Digest(
      R"(Vanilla Policy Gradient is the most basic, entry-level algorithm in the deep RL space because it completely predates the advent of deep RL altogether. The core elements of VPG go all the way back to the late 80s / early 90s. It started a trail of research which ultimately led to stronger algorithms such as TRPO and then PPO soon after.

A key feature of this line of work is that all of these algorithms are on-policy: that is, they don’t use old data, which makes them weaker on sample efficiency. But this is for a good reason: these algorithms directly optimize the objective you care about—policy performance—and it works out mathematically that you need on-policy data to calculate the updates. So, this family of algorithms trades off sample efficiency in favor of stability—but you can see the progression of techniques (from VPG to TRPO to PPO) working to make up the deficit on sample efficiency.)")};

  ASSERT_EQ("2c2ca89ccbfa44169a32172be34361b850500b0dd8121a1ef52d4bb43b767266",
            digest);
}

TEST_F(Sha256Test, CheckByteStream) {
  auto buf = "grape";
  SHA256 hash{};
  for (int i = 0; i < 5; i++) {
    hash.Update((const unsigned char*)buf + i, 1);
  }
  std::string digest = hash.Digest();
  ASSERT_EQ("0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496",
            digest);
}

}  // namespace
}  // namespace crypto
}  // namespace eva
