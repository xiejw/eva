#ifndef LIB_CRON_EXPRESSION_EXPRESSION
#define LIB_CRON_EXPRESSION_EXPRESSION

#include <ctime>

#include "lib/Cron/Expression/Field.h"
#include "lib/Support/Error.h"

namespace eva {
namespace Cron {

class Expression {
 public:
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

  // Finds next matching time, filled in `next_time`.
  Error Next(time_t start_time, time_t *next_time);

 private:
  bool Match(time_t time);

 private:
  std::unique_ptr<Field> minute_;
  std::unique_ptr<Field> hour_;
  std::unique_ptr<Field> day_;
  std::unique_ptr<Field> month_;
  std::unique_ptr<Field> dayOfWeek_;
};

}  // namespace Cron
}  // namespace eva

#endif
