#ifndef EVA_FOUNDATION_ERROR_H_
#define EVA_FOUNDATION_ERROR_H_

namespace eva {

// Boolean type Error return type.
using Error = bool;

constexpr bool kOK = false;
constexpr bool kFAILURE = true;

void FatalError(const char *fmt, ...);

}  // namespace eva

#endif
