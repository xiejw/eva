#include "lib/FileSystem/Utility.h"

namespace eva {
namespace fs {

std::string PathJoin(const char* d_name, const char* f_name) {
  if (strcmp(d_name, "") == 0) return f_name;

  std::string r{d_name};
  if (strcmp(f_name, "") != 0) {
    r += "/";
    r += f_name;
  }
  return r;
}

}  // namespace fs
}  // namespace eva
