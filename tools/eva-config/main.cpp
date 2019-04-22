#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <wordexp.h>

namespace {

void FatalError(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

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

int main(int argc, char** argv) {
  std::string src{"~/Workspace/eva"};
  std::cout << src << " -> " << ExpandPath(src);
  return 0;
}
