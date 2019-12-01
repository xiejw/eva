#include "lib/FileSystem/FileTree.h"

#include <iostream>

#include "lib/FileSystem/Walk.h"

namespace eva {
namespace fs {

void callback(const WalkStat& stat) {
  if (stat.is_folder)
    std::cout << "@\t" << stat.path.c_str() << "\n";
  else
    std::cout << stat.size << "\t" << stat.path.c_str() << "\n";
}

void FileTree::Refresh() { WalkTree(root_path_.c_str(), callback); }

}  // namespace fs
}  // namespace eva