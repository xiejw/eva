add_library(EVAFileSystem
  FileTree.cpp
  FileTree.h
  Walk.cpp
  Walk.h
  Utility.cpp
  Utility.h
  Glob.h
  Glob.cpp
  FileReader.h
  FileReader.cpp
)

set(EVA_FILESYSTEM_LIBS ${EVA_FILESYSTEM_LIBS} EVAFileSystem PARENT_SCOPE)

add_library(TestEVAFileSystem OBJECT
  GlobTest.cpp
  FileReaderTest.cpp
)

target_include_directories(TestEVAFileSystem PUBLIC ${GTEST_INCLUDES})
set(TEST_EVA_FILESYSTEM_LIBS ${TEST_EVA_FILESYSTEM_LIBS} TestEVAFileSystem PARENT_SCOPE)
