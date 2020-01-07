#include "gtest/gtest.h"

#include "eva/FileSystem/FileReader.h"

int get_line_num();

namespace eva::fs {

namespace {

class FileSystemFileReaderTest : public ::testing::Test {};

TEST_F(FileSystemFileReaderTest, CheckNumOfLines) {
  FileReader r{"eva/FileSystem/FileReaderTest.cpp"};
  int line_count = 0;
  while (!r.nextline().consumeValue().end_of_file) line_count++;
  ASSERT_EQ(get_line_num(), line_count);  // Updated accordingly.
}

TEST_F(FileSystemFileReaderTest, CheckFileNotExist) {
  FileReader r{"eva/FileSystem/FileReaderTest.cpp_not_exist"};
  auto status_or = r.nextline();
  ASSERT_FALSE(status_or.ok());
}

TEST_F(FileSystemFileReaderTest, CheckMaxLineLne) {
  FileReader r{"eva/FileSystem/FileReaderTest.cpp", /*max_line_len=*/1};
  auto status_or = r.nextline();
  ASSERT_FALSE(status_or.ok());
  ASSERT_STREQ("Line length is too long.",
               status_or.statusOrDie().message()->c_str());
}

}  // namespace
}  // namespace eva::fs

int get_line_num() { return __LINE__; }  // defined at tail to get line num.
