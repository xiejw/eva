#include <iostream>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  eva::fs::FileTree(".").Refresh();
  return 0;
}
