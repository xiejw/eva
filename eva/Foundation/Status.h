#ifndef EVA_FOUNDATION_STATUS_H_
#define EVA_FOUNDATION_STATUS_H_

#include <memory>
#include <optional>
#include <string>

#include "eva/Foundation/Macros.h"
#include "eva/Foundation/Strings.h"

namespace eva {

enum class ErrorCode {
  InvalidArguments,
  OSError,
  IOError,
};

// A macro to ease providing error message.
// - `err_code` is the preprocessing token of `ErrorCode` above.
// - The rest is for error message. They shall not be empty. And will be passed
//   to Strings::concat directly.
#define EVA_ERROR(err_code, ...) Status(ErrorCode::err_code, __VA_ARGS__)

class Status {
 public:
  // Sets the error code. If present, allows an error message to be set.
  explicit Status(std::optional<ErrorCode> err,
                  std::optional<std::string> msg = std::optional<std::string>{})
      : err_{std::move(err)}, msg_{std::move(msg)} {
    // `msg_` is present only if `err_` is present.
    EVA_CHECK(err_.has_value() || !msg_.has_value());
  };

  explicit Status(ErrorCode err) : err_{err} {};

  template <typename... T>
  explicit Status(ErrorCode err, const T&... args) : err_{err} {
    msg_ = Strings::concat(args...);
  };

 public:
  static const Status OK;

 public:
  // Returns true if no error.
  bool ok() const { return !err_.has_value(); }

  const ErrorCode& errorCode() const { return err_.value(); }
  const std::optional<std::string>& message() const { return msg_; }

 private:
  std::optional<ErrorCode> err_;
  std::optional<std::string> msg_ = {};
};

}  // namespace eva

#endif
