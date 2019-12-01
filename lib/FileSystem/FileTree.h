#ifndef EVA_LIB_FILESYSTEM_FILETREE
#define EVA_LIB_FILESYSTEM_FILETREE

#include <memory>
#include <string>
#include <unordered_map>

namespace eva {
namespace fs {

struct FileHandle {
  std::string path;
  long long size;
};

class FileTree {
 public:
  FileTree(const char* root_path) : root_path_(root_path){};

  // Performs I/O on disk to refresh the content.
  void Refresh();

 private:
  std::string root_path_;
  std::unordered_map<std::string, std::unique_ptr<FileHandle>> map_;
};

}  // namespace fs
}  // namespace eva

#endif
