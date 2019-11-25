#ifndef EVA_LIB_SUPPORT_ERROR
#define EVA_LIB_SUPPORT_ERROR

namespace eva {

using Error = bool;
constexpr bool kOK = false;
constexpr bool kFAILURE = true;

void FatalError(const char *fmt, ...);

}  // namespace eva

#endif
