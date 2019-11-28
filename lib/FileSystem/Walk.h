#ifndef EVA_LIB_FILESYSTEM_WALK
#define EVA_LIB_FILESYSTEM_WALK

#include <string>

namespace eva {
namespace fs {

struct WalkStat {
  // A valid path in the OS. Should be accesiable via current directory of
  // processs.
  std::string full_path;

  // Relative dir path to root.
  std::string d_path;

  // The file (dir) name.
  std::string f_path;

  // Bascially `d_path/f_path`.
  std::string path;

  bool is_folder;
  long long size;
};

using WalkCallback = std::function<void(const WalkStat& stat)>;

void WalkTree(const char* root_path, WalkCallback callback);

}  // namespace fs
}  // namespace eva

#endif
