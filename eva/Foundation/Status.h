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

class Status {
 public:
  // Sets the error code. If present, allows an error message to be set.
  explicit Status(std::optional<ErrorCode> err,
                  std::optional<std::string> msg = std::optional<std::string>{})
      : err_{std::move(err)}, msg_{std::move(msg)} {
    // `msg_` is present only if `err_` is present.
    EVA_CHECK(err_.has_value() || !msg_.has_value());
  };

 public:
  static const Status OK;

  static Status InvalidArguments(std::optional<std::string> msg = {}) {
    return Status(ErrorCode::InvalidArguments, msg);
  };

  template <typename... T>
  static Status InvalidArguments(const T&... args) {
    return Status(ErrorCode::InvalidArguments, Strings::concat(args...));
  };

  static Status OSError(std::optional<std::string> msg = {}) {
    return Status(ErrorCode::OSError, msg);
  };

  template <typename... T>
  static Status OSError(const T&... args) {
    return Status(ErrorCode::OSError, Strings::concat(args...));
  };

  static Status IOError(std::optional<std::string> msg = {}) {
    return Status(ErrorCode::IOError, msg);
  };

  template <typename... T>
  static Status IOError(const T&... args) {
    return Status(ErrorCode::IOError, Strings::concat(args...));
  };

 public:
  // Returns true if no error.
  bool ok() const { return !err_.has_value(); }

  const ErrorCode& errorCode() const { return err_.value(); }
  const std::optional<std::string>& message() const { return msg_; }

 private:
  std::optional<ErrorCode> err_;
  std::optional<std::string> msg_;
};

}  // namespace eva

#endif
