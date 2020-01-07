#ifndef EVA_LIB_CRON_EXPRESSION_FIELD
#define EVA_LIB_CRON_EXPRESSION_FIELD

#include <memory>

#include "eva/Foundation/Errors.h"
#include "lib/Support/Types.h"

namespace eva {
namespace cron {

// Representing a field in cron expression.
class Field {
 private:
  enum Kind { any, singleValue };
  using ValueType = uint;

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

}  // namespace cron
}  // namespace eva

#endif
