#include "eva/Foundation/Errors.h"

#include "gtest/gtest.h"

namespace eva {

namespace {

Error fail() { return kFAILURE; }
Error pass() { return kOK; }

class FoundationErrorTypeTest : public ::testing::Test {};

TEST_F(FoundationErrorTypeTest, CheckSignature) {
  ASSERT_TRUE(fail());
  ASSERT_FALSE(pass());
}

}  // namespace
}  // namespace eva
