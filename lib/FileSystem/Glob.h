#ifndef EVA_LIB_FILESYSTEM_GLOB_
#define EVA_LIB_FILESYSTEM_GLOB_

#include <string>
#include <vector>

namespace eva::fs {

class Glob {
 public:
  Glob(std::string pattern) : pattern_{std::move(pattern)} {}

 public:
  // Retrieves the list of files matching `pattern`.
  void refresh();

  // Returns the result. Must be called after refresh().
  const std::vector<std::string> results() const { return results_; }

 private:
  std::string pattern_;
  std::vector<std::string> results_ = {};
};

}  // namespace eva::fs

#endif
