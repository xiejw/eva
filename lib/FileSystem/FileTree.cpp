#include "lib/FileSystem/FileTree.h"

#include <iostream>

#include "lib/Crypto/Sha256.h"
#include "lib/FileSystem/Walk.h"
#include "lib/Support/Error.h"

namespace eva {
namespace fs {

void FileTree::Refresh() {
  WalkTree(root_path_.c_str(), [&](const WalkStat& stat) {
    if (stat.is_folder) return;

    std::string digest;
    if (option_.fetch_checksum) {
      if (crypto::SHA256::DigestForFile(stat.real_path.c_str(), digest))
        FatalError("Failed to get digest.");
    }

    handles_.push_back(std::unique_ptr<FileHandle>{
        new FileHandle{stat.path, stat.size, {std::move(digest)}}});
  });
}

}  // namespace fs
}  // namespace eva
