#ifndef EVA_FOUNDATION_MACROS_H_
#define EVA_FOUNDATION_MACROS_H_

#include <cassert>

#define EVA_RETURN_IF_ERROR(x)       \
  {                                  \
    auto status = (x);               \
    if (!status.ok()) return status; \
  }

// For the reason why we need two macros here, check
// https://github.com/xiejw/eva/blob/master/docs/notes/iso.md#1933-the--operator-cppconcat
#define EVA_STATUS_MACRO_CONCAT(x, y) EVA_STATUS_MACRO_CONCAT_IMPL(x, y)
#define EVA_STATUS_MACRO_CONCAT_IMPL(x, y) x##y

#define EVA_ASSIGN_OR_RETURN(x, y)                                           \
  EVA_ASSIGN_OR_RETURN_IMPL(EVA_STATUS_MACRO_CONCAT(status_or, __COUNTER__), \
                            x, y)

#define EVA_ASSIGN_OR_RETURN_IMPL(so, x, y) \
  auto so = (y);                            \
  if (!so.ok()) return so.consumeStatus();  \
  auto x = so.consumeValue();

#define EVA_CHECK(x) assert(x)

// Fail the check with an error message.
#define EVA_FAIL(err_msg) assert(((err_msg) && false))

#endif
