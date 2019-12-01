#ifndef EVA_LIB_FILESYSTEM_FILETREE
#define EVA_LIB_FILESYSTEM_FILETREE

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace eva {
namespace fs {

struct FileHandle {
  std::string path;
  long long size;
};

class FileTree {
 public:
  FileTree(const char* root_path) : root_path_(root_path), handles_{} {};

  // Performs I/O on disk to refresh the content.
  void Refresh();

 public:
  using file_handle_t = std::unique_ptr<const FileHandle>;
  using const_iterator = std::vector<file_handle_t>::const_iterator;

  const_iterator begin() const { return handles_.cbegin(); };
  const_iterator end() const { return handles_.cend(); };

 private:
  std::string root_path_;
  std::vector<file_handle_t> handles_;
  std::unordered_map<std::string, std::unique_ptr<FileHandle>> map_;
};

}  // namespace fs
}  // namespace eva

#endif
