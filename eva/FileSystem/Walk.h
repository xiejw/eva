#ifndef EVA_FILESYSTEM_WALK_H_
#define EVA_FILESYSTEM_WALK_H_

#include <functional>
#include <string>

namespace eva {
namespace fs {

struct WalkStat {
  // The real path in the OS. Should be accesiable via current directory of
  // processs.
  std::string real_path;

  // Relative dir path to root.
  std::string d_path;

  // The file (dir) name.
  std::string f_path;

  // Bascially `d_path/f_path`.
  std::string path;

  bool is_folder;
  long long size;
};

enum class WalkAction {
  // Default. Continue current walking routing.
  kContinue,

  // Only operate on the folder. Do not dive into.
  kDoNotDiveIntoFolder,

  // Skip of the rest of walking. For any parent folder, it will skip also,
  // i.e., abort protocol.
  kSkipRest,
};

using WalkCallback = std::function<WalkAction(const WalkStat& stat)>;

void WalkTree(const char* root_path, WalkCallback callback);

}  // namespace fs
}  // namespace eva

#endif
