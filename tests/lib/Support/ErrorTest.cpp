#include "gtest/gtest.h"
#include "lib/Support/Error.h"

namespace eva {

namespace {

Error fail() { return kFAILURE; }
Error pass() { return kOK; }

class ErrorTypeTest : public ::testing::Test {};

TEST_F(ErrorTypeTest, CheckSignature) {
  ASSERT_TRUE(fail());
  ASSERT_FALSE(pass());
}

}  // namespace
}  // namespace eva
