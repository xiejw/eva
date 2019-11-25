add_library(FileSystem
  Walk.cpp
  Walk.h
)

set(FILESYSTEM_LIBS ${FILESYSTEM_LIBS} FileSystem PARENT_SCOPE)
