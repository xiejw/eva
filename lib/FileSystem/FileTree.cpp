#include "lib/FileSystem/FileTree.h"

#include <iostream>

#include "lib/FileSystem/Walk.h"

namespace eva {
namespace fs {

// void callback(const WalkStat& stat) {
//   if (stat.is_folder)
//     std::cout << "@\t" << stat.path.c_str() << "\n";
//   else
//     std::cout << stat.size << "\t" << stat.path.c_str() << "\n";
// }

void FileTree::Refresh() {
  WalkTree(root_path_.c_str(), [&](const WalkStat& stat) {
    if (stat.is_folder) return;
    handles_.push_back(
        std::unique_ptr<FileHandle>{new FileHandle{stat.path, stat.size}});
  });
}

}  // namespace fs
}  // namespace eva
