#ifndef EVA_LIB_FILESYSTEM_FILETREE
#define EVA_LIB_FILESYSTEM_FILETREE

#include <string>

namespace eva {
namespace fs {

class FileTree {

  public:
    FileTree(const char* root_path): root_path_(root_path) {};

    // Performs I/O on disk to refresh the content.
    void Refresh();

  private:
    std::string root_path_;
    std::string map_;
};

}  // namespace fs
}  // namespace eva

#endif
