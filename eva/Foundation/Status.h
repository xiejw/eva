#ifndef EVA_FOUNDATION_STATUS_H_
#define EVA_FOUNDATION_STATUS_H_

#include <memory>
#include <optional>
#include <string>

#include "eva/Foundation/Macros.h"

namespace eva {

enum class ErrorCode {
  InvalidArguments,
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

  static const Status OK;

  static Status InvalidArguments(std::optional<std::string> msg = {}) {
    return Status(ErrorCode::InvalidArguments, msg);
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
