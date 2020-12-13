#include "eva/Foundation/Errors.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace eva {

void FatalError(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

}  // namespace eva
