#include "gtest/gtest.h"

#include "lib/FileSystem/Glob.h"

namespace eva::fs {

namespace {

class GlobTest : public ::testing::Test {};

TEST_F(GlobTest, CheckFiles) {
  Glob gb{"lib/FileSystem/*Test.cpp"};
  gb.refresh();
  ASSERT_EQ(1, gb.results().size());
  // Should find myself.
  ASSERT_TRUE(gb.results()[0].rfind("GlobTest.cpp") != std::string::npos);
}

}  // namespace
}  // namespace eva::fs
