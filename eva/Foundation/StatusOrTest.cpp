#include "eva/Foundation/StatusOr.h"

#include "gtest/gtest.h"

namespace eva {

namespace {

class FoundationStatusOrTest : public ::testing::Test {};

TEST_F(FoundationStatusOrTest, CheckStatus) {
  auto status_or = StatusOr<std::string>{Status{ErrorCode::InvalidArguments}};
  ASSERT_FALSE(status_or.ok());

  auto status = status_or.statusOrDie();
  ASSERT_EQ(ErrorCode::InvalidArguments, status.errorCode());
  ASSERT_FALSE(status.message().has_value());
}

TEST_F(FoundationStatusOrTest, CheckValue) {
  auto status_or = StatusOr<std::string>{"hello"};
  ASSERT_TRUE(status_or.ok());
  ASSERT_STREQ("hello", status_or.valueOrDie().c_str());
}

TEST_F(FoundationStatusOrTest, ConsumeValue) {
  auto status_or = StatusOr<std::string>{"hello"};
  ASSERT_TRUE(status_or.ok());

  auto value = status_or.consumeValue();
  ASSERT_STREQ("hello", value.c_str());
}

}  // namespace

}  // namespace eva
