#include "lib/FileSystem/Glob.h"

#include <glob.h>

namespace eva::fs {

void Glob::refresh() {
  glob_t globbuf;
  glob(pattern_.c_str(), GLOB_NOSORT, NULL, &globbuf);

  int total = globbuf.gl_pathc;

  results_.clear();
  results_.reserve(total);

  for (int i = 0; i < total; ++i) results_.push_back(globbuf.gl_pathv[i]);
}

}  // namespace eva::fs
