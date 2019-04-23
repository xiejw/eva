#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include <wordexp.h>

constexpr const char* kEvaPath = "~/Workspace/eva";

namespace {

/// Reports the fatal error to `stderr` and exits immediately.
void FatalError(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

/// Expands the `path` according to the shell-style.
std::string ExpandPath(const std::string& path) {
  wordexp_t exp_result;
  if (int error = wordexp(path.c_str(), &exp_result, 0)) {
    FatalError("Failed to expand the path: %s", path.c_str());
  }
  std::string result{exp_result.we_wordv[0]};
  wordfree(&exp_result);
  return result;
}

}  // namespace

namespace {

struct Formula {
  std::string name;
  std::string cxxflags;
  std::string ldflags;

  Formula(const char* a_name, const char* a_cxxflags, const char* a_ldflags)
      : name{a_name}, cxxflags{a_cxxflags}, ldflags{a_ldflags} {}
};

struct OutputInfo {
  bool print_cxxflags;
  bool print_ldflags;
  Formula* formula;
};

class Outputs {
 public:
  Outputs() : formulars_{}, infos_{} {
    // Initialze the information table. Right now, we just hard code them.
    formulars_.push_back(std::make_unique<Formula>(
        /*name=*/"gflags",
        /*cxxflags=*/"-Idependencies/gflags/buildlib/include/",
        /*ldflags=*/"dependencies/gflags/buildlib/lib/libgflags_nothreads.a"));

    formulars_.push_back(std::make_unique<Formula>(
        /*name=*/"googletest",
        /*cxxflags=*/"-Idependencies/googletest/googletest/include/",
        /*ldflags=*/
        "dependencies/googletest/buildlib/lib/libgtest.a -lpthread"));
  }

 public:
  void Print() {}

 private:
  std::vector<std::unique_ptr<Formula>> formulars_;
  std::vector<std::unique_ptr<OutputInfo>> infos_;
};

}  // namespace

int main(int argc, char** argv) {
  std::string src{kEvaPath};
  std::cout << src << " -> " << ExpandPath(src);
  return 0;
}
