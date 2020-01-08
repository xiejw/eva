#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>

#include "eva/FileSystem/Utility.h"
#include "eva/FileSystem/Walk.h"
#include "eva/Foundation/Errors.h"

namespace eva {
namespace fs {

namespace {

WalkAction ListFiles(const char*, const char*, WalkCallback callback);

WalkAction PrintFileStat(const char* root_path, const WalkStat& stat,
                         WalkCallback callback) {
  // Common fiter
  if (stat.f_path.find(".", 0) == 0) return WalkAction::kContinue;

  auto action = callback(stat);
  switch (action) {
    case WalkAction::kContinue:
      if (stat.is_folder) {
        ListFiles(root_path, stat.path.c_str(), callback);
      }
      return action;
    case WalkAction::kDoNotDiveIntoFolder:
      // We just ignore the folder action. So rewrites as kContinue.
      return WalkAction::kContinue;
    case WalkAction::kSkipRest:
      return action;
  }
}

WalkAction ListFiles(const char* root_path, const char* relative_d,
                     WalkCallback callback) {
  auto d_path = PathJoin(root_path, relative_d);
  DIR* dirp;
  dirp = opendir(d_path.c_str());
  if (dirp == nullptr) {
    eva::FatalError("opendir failed on '%s'", d_path.c_str());
    return WalkAction::kSkipRest;
  }

  std::vector<std::unique_ptr<WalkStat>> files{};
  {
    struct dirent* dp;
    for (;;) {
      errno = 0;  // To distinguish error from end-of-directory
      dp = readdir(dirp);
      if (dp == nullptr) break;

      if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        continue;  // Skip . and ..

      {
        struct stat sb;
        auto f_path = dp->d_name;
        auto real_path = PathJoin(d_path.c_str(), f_path);
        if (stat(real_path.c_str(), &sb) == -1) {
          printf("error stat %s: %d\n", real_path.c_str(), errno);
          continue;
        }

        auto* st = new WalkStat{/*real_path=*/real_path,
                                /*d_path*/ relative_d,
                                /*f_path=*/f_path,
                                /*path=*/PathJoin(relative_d, f_path),
                                /* is_folder =*/dp->d_type == DT_DIR,
                                /*size=*/sb.st_size};
        files.push_back(std::unique_ptr<WalkStat>{st});
      }
    }
    if (errno != 0) eva::FatalError("readdir");

    if (closedir(dirp) == -1) eva::FatalError("closedir");
  }

  std::sort(
      files.begin(), files.end(),
      [](const std::unique_ptr<WalkStat>& a,
         const std::unique_ptr<WalkStat>& b) { return a->path < b->path; });

  for (auto& stat : files) {
    auto action = PrintFileStat(root_path, *stat, callback);
    // kDoNotDiveIntoFolder is not a valid action here.
    assert(action == WalkAction::kContinue || action == WalkAction::kSkipRest);
    if (action == WalkAction::kSkipRest) {
      return action;
    }
  }

  return WalkAction::kContinue;
}
}  // namespace

void WalkTree(const char* root_path, WalkCallback callback) {
  ListFiles(root_path, "", callback);
}

}  // namespace fs
}  // namespace eva
