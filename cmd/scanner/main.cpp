#include <dirent.h>
#include <iostream>
#include <cstring>

#include "lib/Support/Error.h"

namespace {
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

    printf("%s", dp->d_name);
    if (dp->d_type == DT_DIR) printf("@D");

    printf("\n");
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
