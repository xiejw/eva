#include "lib/FileSystem/FileTree.h"

#include <iostream>

#include "lib/Crypto/Sha256.h"
#include "lib/FileSystem/Walk.h"
#include "lib/Support/Error.h"

namespace eva {
namespace fs {

void FileTree::Refresh() {
  int max_num_files = bool(option_.max_num_files) ? option_.max_num_files.value() : -1;
  int num_files_visited = 0;

  WalkTree(root_path_.c_str(), [&](const WalkStat& stat) {
    if (stat.is_folder) return;

    // We should have a way to fast stop the walk.
    if (max_num_files != -1 && num_files_visited >= max_num_files) return;

    std::string digest;
    if (option_.fetch_checksum) {
      if (crypto::SHA256::DigestForFile(stat.real_path.c_str(), digest))
        FatalError("Failed to get digest.");
    }

    handles_.push_back(std::unique_ptr<FileHandle>{
        new FileHandle{stat.path, stat.size, {std::move(digest)}}});
    num_files_visited++;
  });
}

}  // namespace fs
}  // namespace eva
