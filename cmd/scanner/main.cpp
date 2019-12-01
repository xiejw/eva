#include <iostream>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  // eva::fs::FileTree tree{".", {/*fetch_checksum=*/true}};
  eva::fs::FileTree tree{"."};
  tree.Refresh();
  for (auto& handle : tree) {
    std::cout << handle->size << "\t" << handle->path << "\n";
  }
  return 0;
}
