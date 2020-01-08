#include "gtest/gtest.h"

#include "eva/FileSystem/Glob.h"

namespace eva::fs {

namespace {

class FileSystemGlobTest : public ::testing::Test {};

TEST_F(FileSystemGlobTest, CheckFiles) {
  Glob gb{"eva/FileSystem/G*Test.cpp"};
  gb.refresh();
  ASSERT_EQ(1, gb.results().size());
  // Should find myself.
  ASSERT_TRUE(gb.results()[0].rfind("GlobTest.cpp") != std::string::npos);
}

}  // namespace
}  // namespace eva::fs
