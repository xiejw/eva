#include "gtest/gtest.h"

#include "eva/FileSystem/FileReader.h"

namespace eva::fs {

namespace {

class FileSystemFileReaderTest : public ::testing::Test {};

TEST_F(FileSystemFileReaderTest, CheckNumOfLines) {
  FileReader r{"eva/FileSystem/FileReaderTest.cpp"};
  int line_count = 0;
  while (!r.nextline().consumeValue().end_of_file) line_count++;
  ASSERT_EQ(19, line_count);
}

}  // namespace
}  // namespace eva::fs
