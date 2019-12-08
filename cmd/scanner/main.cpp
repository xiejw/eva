#include <iostream>

#include "lib/FileSystem/FileSystem.h"

constexpr bool kFetchChecksum = true;
constexpr int kMaxNumFiles = 5;

int main(int argc, char** argv) {
  eva::fs::FileTree tree{".", {kFetchChecksum, kMaxNumFiles}};
  tree.Refresh();

  for (auto& handle : tree) {
    if (kFetchChecksum)
      std::cout << handle->checksum.value() << " " << handle->path << "\n";
    else
      std::cout << handle->size << "\t" << handle->path << "\n";
  }
  return 0;
}
