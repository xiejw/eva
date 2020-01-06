#include "mlvm/Foundation/Status.h"

#include "gtest/gtest.h"

namespace mlvm::foundation {

namespace {

class StatusTest : public ::testing::Test {};

TEST_F(StatusTest, CheckOK) {
  auto status = Status::OK;
  ASSERT_TRUE(status.ok());
}

TEST_F(StatusTest, CheckErrorCode) {
  auto status = Status(ErrorCode::InvalidArguments, "Hello");
  ASSERT_EQ(ErrorCode::InvalidArguments, status.errorCode());
}

TEST_F(StatusTest, CheckErrorMessage) {
  auto status = Status(ErrorCode::InvalidArguments, "Hello");
  ASSERT_STREQ("Hello", status.message().value().c_str());

  status = Status(ErrorCode::InvalidArguments);
  ASSERT_FALSE(status.message().has_value());
}

}  // namespace

}  // namespace mlvm::foundation
