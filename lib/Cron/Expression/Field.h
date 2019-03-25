#ifndef LIB_CRON_EXPRESSION_FIELD
#define LIB_CRON_EXPRESSION_FIELD

#include <memory>
static void FatalError(const char *fmt, ...) {
  // fprintf(stderr, "%s: ", progname.c_str());
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

class Field {
  enum Kind {any, singleValue};

  using ValueType = uint;

 public:
  Field() = delete;

  static std::unique_ptr<Field> MakeAny() {
    auto* field = new Field(any, 0);
    return std::unique_ptr<Field>(field);
  }

  static std::unique_ptr<Field> MakeSingleValue(ValueType value) {
    auto* field = new Field(singleValue, value);
    return std::unique_ptr<Field>(field);
  }

 public:
  Kind kind() { return kind_; }

  ValueType value() { return value_; }

  bool Match(ValueType value) {
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
  Field(Kind kind, ValueType value): kind_(kind), value_(value) {};

  Kind kind_;
  ValueType value_;

};

#endif
