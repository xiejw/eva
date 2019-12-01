#include <iostream>
#include <optional>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  eva::fs::FileTree tree{"."};
  tree.Refresh();
  for (auto& handle : tree) {
    std::cout << handle->path << "" << bool(std::optional<std::string>{}) << "\n";
  }
  return 0;
}
