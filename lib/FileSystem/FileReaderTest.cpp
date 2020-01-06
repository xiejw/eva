#include "gtest/gtest.h"

#include "lib/FileSystem/FileReader.h"

namespace eva::fs {

namespace {

class FileSystemFileReaderest : public ::testing::Test {};

TEST_F(FileSystemFileReaderest, CheckNumOfLines) {
  FileReader r{"lib/FileSystem/FileReaderTest.cpp"};
  int line_count = 0;
  while (r.nextline()) line_count++;
  ASSERT_EQ(19, line_count);
}

}  // namespace
}  // namespace eva::fs
