#include "gtest/gtest.h"
#include "lib/Cron/Expression/Expression.h"
#include "lib/Cron/Expression/Field.h"

namespace eva {
namespace Cron {

namespace {

class ExpressionTest : public ::testing::Test {
 public:
  time_t NextTime(Expression& expression, time_t start_time) {
    time_t next_time;
    expression.Next(start_time, &next_time);
    return next_time;
  }
};

TEST_F(ExpressionTest, CheckMinute) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/* minute =*/Field::MakeSingleValue(40));

  // Wednesday, March 27, 2019 11:40:00 AM GMT-07:00
  ASSERT_EQ(1553712000, NextTime(expression, start_time));
}

}  // namespace
}  // namespace Cron
}  // namespace eva
