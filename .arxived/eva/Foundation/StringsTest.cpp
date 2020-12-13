#include "eva/Foundation/Strings.h"

#include "gtest/gtest.h"

namespace eva {

namespace {

class FoundationStringsTest : public ::testing::Test {};

TEST_F(FoundationStringsTest, CheckOneArg) {
  ASSERT_STREQ("Hello", Strings::concat("Hello").c_str());
}

TEST_F(FoundationStringsTest, CheckTwoArgs) {
  ASSERT_STREQ("1a", Strings::concat(1, "a").c_str());
}

TEST_F(FoundationStringsTest, CheckThreeArgs) {
  ASSERT_STREQ("1ab", Strings::concat(1, "a", std::string{"b"}).c_str());
}

}  // namespace

}  // namespace eva
