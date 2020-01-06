#ifndef EVA_FOUNDATION_STATUSOR_H_
#define EVA_FOUNDATION_STATUSOR_H_

#include <optional>

#include "eva/Foundation/Macros.h"
#include "eva/Foundation/Status.h"

namespace mlvm {

template <class T>
class StatusOr {
 public:
  StatusOr(T&& t) : value_{std::move(t)} {};

  StatusOr(Status&& status) : status_{std::move(status)} {
    EVA_CHECK(status_.has_value());
    EVA_CHECK(!status_.value().ok());
  };

  StatusOr(StatusOr&&) = default;
  StatusOr& operator=(StatusOr&&) = default;

  // Copy is now allowed.
  StatusOr(const StatusOr&) = delete;
  StatusOr& operator=(const StatusOr&) = delete;

 public:
  bool ok() const { return value_.has_value(); };

  // Requests ok() == false
  const Status& statusOrDie() const {
    AssertNotHoldValue();
    return status_.value();
  };

  // Requests ok() == true
  T& valueOrDie() {
    AssertHoldValue();
    AssertNotReleased();
    return value_.value();
  };

  // Requests ok() == false. Should be called at most once.
  Status&& consumeStatus() {
    AssertNotHoldValue();
    return std::move(status_.value());
  };

  // Requests ok() == true. Should be called at most once.
  T&& consumeValue() {
    AssertHoldValue();
    AssertNotReleased();
    Release();
    return std::move(value_.value());
  };

 private:
  std::optional<T> value_ = {};
  std::optional<Status> status_ = {};

 private:
#ifndef NDEBUG
  bool released_ = false;
  void inline AssertNotReleased() const { EVA_CHECK(!released_); };
  void inline Release() { released_ = true; }
  void inline AssertHoldValue() const { EVA_CHECK(value_.has_value()); };
  void inline AssertNotHoldValue() const { EVA_CHECK(!value_.has_value()); };
#else
  void inline AssertNotReleased() const {};
  void inline Release(){};
  void inline AssertHoldValue() const {};
  void inline AssertNotHoldValue() const {};
#endif
};

}  // namespace mlvm

#endif
