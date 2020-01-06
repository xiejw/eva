add_library(EVAFileSystem
  FileReader.h
  FileReader.cpp
)

set(FILESYSTEM_LIBS ${FILESYSTEM_LIBS} EVAFileSystem PARENT_SCOPE)

add_library(TestEVAFileSystem OBJECT
  FileReaderTest.cpp
)

target_include_directories(TestEVAFileSystem PUBLIC ${GTEST_INCLUDES})
set(TEST_FILESYSTEM_LIBS ${TEST_FILESYSTEM_LIBS} TestEVAFileSystem PARENT_SCOPE)
