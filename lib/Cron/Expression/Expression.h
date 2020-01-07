#ifndef EVA_LIB_CRON_EXPRESSION_EXPRESSION
#define EVA_LIB_CRON_EXPRESSION_EXPRESSION

#include <ctime>

#include "eva/Foundation/Errors.h"
#include "lib/Cron/Expression/Field.h"

namespace eva {
namespace cron {

class Expression {
 public:
  using Time = time_t;

  // Construcotr. Defaults to `* * * * *`.
  Expression(std::unique_ptr<Field> minute = Field::MakeAny(),
             std::unique_ptr<Field> hour = Field::MakeAny(),
             std::unique_ptr<Field> day = Field::MakeAny(),
             std::unique_ptr<Field> month = Field::MakeAny(),
             std::unique_ptr<Field> dayOfWeek = Field::MakeAny())
      : minute_(std::move(minute)),
        hour_(std::move(hour)),
        day_(std::move(day)),
        month_(std::move(month)),
        dayOfWeek_(std::move(dayOfWeek)) {}

  // Finds next matching time, filled in `next_time`, starting from
  // `start_time`.
  Error Next(Time start_time, Time *next_time);

 private:
  // Tests whether `time` matches the expression.
  bool Match(Time time);

 private:
  std::unique_ptr<Field> minute_;
  std::unique_ptr<Field> hour_;
  std::unique_ptr<Field> day_;
  std::unique_ptr<Field> month_;
  std::unique_ptr<Field> dayOfWeek_;
};

}  // namespace cron
}  // namespace eva

#endif
