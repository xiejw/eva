#include <iostream>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  eva::fs::FileTree tree{"."};
  tree.Refresh();
  for (auto& handle : tree) {
    std::cout << handle->path << "\n";
  }
  return 0;
}
