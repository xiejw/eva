#include <iostream>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  bool fetch_checksum = true;

  eva::fs::FileTree tree{".", {fetch_checksum}};
  tree.Refresh();

  for (auto& handle : tree) {
    if (fetch_checksum)
      std::cout << handle->checksum.value() << " " << handle->path << "\n";
    else
      std::cout << handle->size << "\t" << handle->path << "\n";
  }
  return 0;
}
