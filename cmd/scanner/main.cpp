#include <dirent.h>
#include <cstring>
#include <iostream>
#include <string>

#include "lib/Support/Error.h"

namespace {

struct Stat {
  std::string f_path;
  bool is_folder;
};

void PrintFileStat(const Stat& stat) {
  std::cout << stat.f_path;
  if (stat.is_folder) std::cout << "@D";
  std::cout << "\n";
}

void ListFiles(const char* dirpath) {
  DIR* dirp;
  dirp = opendir(dirpath);
  if (dirp == nullptr) {
    eva::FatalError("opendir failed on '%s'", dirpath);
    return;
  }

  struct dirent* dp;
  for (;;) {
    errno = 0;  // To distinguish error from end-of-directory
    dp = readdir(dirp);
    if (dp == nullptr) break;

    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue;  // Skip . and ..

    // d_type is not POSIX.
    PrintFileStat(
        Stat{/* f_path =*/dp->d_name, /* is_folder =*/dp->d_type == DT_DIR});
  }

  if (errno != 0) eva::FatalError("readdir");

  if (closedir(dirp) == -1) eva::FatalError("closedir");
}
}  // namespace

int main(int argc, char** argv) {
  std::cout << "Hello\n";
  ListFiles(".");
  return 0;
}
