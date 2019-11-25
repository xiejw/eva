#include <cstring>
#include <iostream>
#include <string>

#include <dirent.h>
#include <sys/stat.h>

#include "lib/FileSystem/Walk.h"
#include "lib/Support/Error.h"

namespace eva {
namespace fs {

namespace {

std::string PathJoin(const char* dir, const char* f_name) {
  if (dir == nullptr || strcmp(dir, "") == 0) return f_name;

  std::string r{dir};
  if (f_name != nullptr && strcmp(f_name, "") != 0) {
    r += "/";
    r += f_name;
  }
  return r;
}

void ListFiles(const char*, const char*);

void PrintFileStat(const char* root_path, const WalkStat& stat) {
  // Common fiter
  if (stat.is_folder && stat.f_path.find(".", 0) == 0) return;

  auto relative_path = PathJoin(stat.d_path.c_str(), stat.f_path.c_str());
  if (stat.is_folder)
    std::cout << "@\t" << relative_path.c_str();
  else
    std::cout << stat.size << "\t" << relative_path.c_str();
  std::cout << "\n";

  if (stat.is_folder) {
    ListFiles(root_path, relative_path.c_str());
  }
}

void ListFiles(const char* root_path, const char* relative_d) {
  auto d_path = PathJoin(root_path, relative_d);
  DIR* dirp;
  dirp = opendir(d_path.c_str());
  if (dirp == nullptr) {
    eva::FatalError("opendir failed on '%s'", d_path.c_str());
    return;
  }

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

      WalkStat st{/*full_path=*/full_path,
                  /*d_path*/ (relative_d == nullptr) ? "" : relative_d,
                  /*f_path=*/f_path,
                  /* is_folder =*/dp->d_type == DT_DIR,
                  /*size=*/sb.st_size};
      PrintFileStat(root_path, st);
    }
  }

  if (errno != 0) eva::FatalError("readdir");

  if (closedir(dirp) == -1) eva::FatalError("closedir");
}
}  // namespace

void WalkTree(const char* root_path) { ListFiles(root_path, nullptr); }

}  // namespace fs
}  // namespace eva
