#ifndef EVA_FOUNDATION_MACROS_H_
#define EVA_FOUNDATION_MACROS_H_

#include <cassert>

// Wrapper for `assert`
#define EVA_CHECK(x) assert(x)

// Fail the check in debug mode with an error message displayed.
#define EVA_FAIL(err_msg) assert(((err_msg) && false))

// A macro to ease providing error message.
// - `err_code` is the preprocessing token of `ErrorCode` defined in Status.h.
// - The rest is for error message. It can be empty. If present, will be passed
//   to `Strings::concat` directly.
#define EVA_ERROR(err_code, ...) \
  Status { ErrorCode::err_code __VA_OPT__(, ) __VA_ARGS__ }

// Returns the function with error status if x.ok() == false.
#define EVA_RETURN_IF_ERROR(x)       \
  {                                  \
    auto status = (x);               \
    if (!status.ok()) return status; \
  }

// Returns the function with error status if status_or.ok() == false, where
// status_or = v.
//
// Otherwise, assigns the value to `x` (i.e., status_or.ok() == true).
#define EVA_ASSIGN_OR_RETURN(x, v)                                             \
  EVA_ASSIGN_OR_RETURN_IMPL(EVA_STATUS_MACRO_CONCAT(__status_or, __COUNTER__), \
                            x, v)

///////////////////////////////////////////////////////////////////////////////
// Begin with helper macros.
//
///////////////////////////////////////////////////////////////////////////////

// For the reason why we need two macros here, check
// https://github.com/xiejw/eva/blob/master/docs/notes/iso.md#1933-the--operator-cppconcat
#define EVA_STATUS_MACRO_CONCAT(x, y) EVA_STATUS_MACRO_CONCAT_IMPL(x, y)
#define EVA_STATUS_MACRO_CONCAT_IMPL(x, y) x##y

#define EVA_ASSIGN_OR_RETURN_IMPL(so, x, y) \
  auto so = (y);                            \
  if (!so.ok()) return so.consumeStatus();  \
  auto x = so.consumeValue();

#endif
