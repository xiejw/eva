#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>

#include "lib/FileSystem/Utility.h"
#include "lib/FileSystem/Walk.h"
#include "lib/Support/Error.h"

namespace eva {
namespace fs {

namespace {

void ListFiles(const char*, const char*, WalkCallback callback);

void PrintFileStat(const char* root_path, const WalkStat& stat,
                   WalkCallback callback) {
  // Common fiter
  if (stat.f_path.find(".", 0) == 0) return;

  callback(stat);

  if (stat.is_folder) {
    ListFiles(root_path, stat.path.c_str(), callback);
  }
}

void ListFiles(const char* root_path, const char* relative_d,
               WalkCallback callback) {
  auto d_path = PathJoin(root_path, relative_d);
  DIR* dirp;
  dirp = opendir(d_path.c_str());
  if (dirp == nullptr) {
    eva::FatalError("opendir failed on '%s'", d_path.c_str());
    return;
  }

  {
    std::vector<std::unique_ptr<WalkStat>> files{};
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
        auto full_path = PathJoin(d_path.c_str(), f_path);
        if (stat(full_path.c_str(), &sb) == -1) {
          printf("error stat %s: %d\n", full_path.c_str(), errno);
          continue;
        }

        auto* st = new WalkStat{/*full_path=*/full_path,
                                /*d_path*/ relative_d,
                                /*f_path=*/f_path,
                                /*path=*/PathJoin(relative_d, f_path),
                                /* is_folder =*/dp->d_type == DT_DIR,
                                /*size=*/sb.st_size};
        files.push_back(std::unique_ptr<WalkStat>{st});
      }
    }

    std::sort(
        files.begin(), files.end(),
        [](const std::unique_ptr<WalkStat>& a,
           const std::unique_ptr<WalkStat>& b) { return a->path < b->path; });

    for (auto& stat : files) {
      PrintFileStat(root_path, *stat, callback);
    }
  }

  if (errno != 0) eva::FatalError("readdir");

  if (closedir(dirp) == -1) eva::FatalError("closedir");
}
}  // namespace

void WalkTree(const char* root_path, WalkCallback callback) {
  ListFiles(root_path, "", callback);
}

}  // namespace fs
}  // namespace eva
