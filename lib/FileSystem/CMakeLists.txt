add_library(FileSystem
  FileTree.cpp
  FileTree.h
  Walk.cpp
  Walk.h
  Utility.cpp
  Utility.h
  Glob.h
  Glob.cpp
)

set(FILESYSTEM_LIBS ${FILESYSTEM_LIBS} FileSystem PARENT_SCOPE)

add_library(TestFileSystem OBJECT
  GlobTest.cpp
)

target_include_directories(TestFileSystem PUBLIC ${GTEST_INCLUDES})
set(TEST_FILESYSTEM_LIBS ${TEST_FILESYSTEM_LIBS} TestFileSystem PARENT_SCOPE)
