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
