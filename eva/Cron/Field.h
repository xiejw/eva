#ifndef EVA_CRON_FIELD_H_
#define EVA_CRON_FIELD_H_

#include <memory>

#include "eva/Foundation/Errors.h"

namespace eva::cron {

// Representing a field in cron expression.
class Field {
 private:
  enum Kind { any, singleValue };
  using ValueType = unsigned int;

 public:
  // Use factory methods instead.
  Field() = delete;

  // Match `*`.
  static std::unique_ptr<Field> MakeAny() {
    auto *field = new Field(any, 0);
    return std::unique_ptr<Field>(field);
  }

  // Match a single value.
  static std::unique_ptr<Field> MakeSingleValue(ValueType value) {
    auto *field = new Field(singleValue, value);
    return std::unique_ptr<Field>(field);
  }

  // Tests whether `value` matches this field.
  bool Match(ValueType value) const {
    switch (kind_) {
      case any:
        return true;
      case singleValue:
        return value_ == value;
      default:
        FatalError("Field kind %d is not expected.", kind_);
    }
  }

 private:
  Field(Kind kind, ValueType value) : kind_(kind), value_(value){};

  Kind kind() { return kind_; }

  ValueType value() { return value_; }

 private:
  Kind kind_;
  ValueType value_;
};

}  // namespace eva::cron

#endif
