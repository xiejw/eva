#ifndef EVA_LIB_FILESYSTEM_FILETREE
#define EVA_LIB_FILESYSTEM_FILETREE

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace eva {
namespace fs {

struct FileHandle {
  std::string path;
  long long size;
  std::optional<std::string> checksum;
};

class FileTree {
 public:
  struct Option {
    // Fetches checksum and fills into `FileHandle` during `Refresh`.
    bool fetch_checksum;
  };

 public:
  FileTree(const char* root_path, const Option& option = {})
      : root_path_(root_path), option_{option}, handles_{} {};

 public:
  using file_handle_t = std::unique_ptr<const FileHandle>;
  using const_iterator = std::vector<file_handle_t>::const_iterator;

  // Range loop support.
  const_iterator begin() const { return handles_.cbegin(); };
  const_iterator end() const { return handles_.cend(); };

  // Performs I/O on disk to refresh the content.
  void Refresh();

 private:
  std::string root_path_;
  const Option option_;

  std::vector<file_handle_t> handles_;
  std::unordered_map<std::string, std::unique_ptr<FileHandle>> map_;
};

}  // namespace fs
}  // namespace eva

#endif
