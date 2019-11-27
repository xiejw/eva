#ifndef EVA_LIB_FILESYSTEM_WALK
#define EVA_LIB_FILESYSTEM_WALK

#include <string>

namespace eva {
namespace fs {

struct WalkStat {
  // A valid path in the OS. Should be accesiable via current directory of
  // processs.
  std::string full_path;

  // Relative path to root.
  std::string d_path;

  // The file (dir) name.
  std::string f_path;

  bool is_folder;
  long long size;
};

void WalkTree(const char* root_path, void (*callback)(const WalkStat& stat));

}  // namespace fs
}  // namespace eva

#endif
