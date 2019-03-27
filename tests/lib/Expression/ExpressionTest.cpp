#include <cstdlib>

#include "gtest/gtest.h"
#include "lib/Cron/Expression/Expression.h"
#include "lib/Cron/Expression/Field.h"

namespace eva {
namespace Cron {

namespace {

class ExpressionTest : public ::testing::Test {
 public:
  void SetUp() override {
    // All the epoch values here are based on America/Los_Angeles.
    tz_value_ = getenv("TZ");
    setenv("TZ", "America/Los_Angeles", /*overwrite = */ 1);
  }

  void TearDown() override {
    if (tz_value_) {
      setenv("TZ", tz_value_, /*overwrite = */ 1);
    } else {
      unsetenv("TZ");
    }
  }
  time_t NextTime(Expression& expression, time_t start_time) {
    time_t next_time;
    expression.Next(start_time, &next_time);
    return next_time;
  }

 private:
  char* tz_value_ = nullptr;
};

TEST_F(ExpressionTest, CheckMinute) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/*minute = */ Field::MakeSingleValue(40));

  // Wednesday, March 27, 2019 11:40:00 AM GMT-07:00
  ASSERT_EQ(1553712000, NextTime(expression, start_time));
}

TEST_F(ExpressionTest, CheckHour) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/*minute = */ Field::MakeAny(),
                               /*hour = */ Field::MakeSingleValue(13));

  // Wednesday, March 27, 2019 1:00:00 PM GMT-07:00
  ASSERT_EQ(1553716800, NextTime(expression, start_time));
}

TEST_F(ExpressionTest, CheckHourInNextDay) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/*minute = */ Field::MakeAny(),
                               /*hour = */ Field::MakeSingleValue(9));

  // Thursday, March 28, 2019 9:00:00 AM GMT-07:00
  ASSERT_EQ(1553788800, NextTime(expression, start_time));
}

TEST_F(ExpressionTest, EndToEnd) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/*minute = */ Field::MakeSingleValue(13),
                               /*hour = */ Field::MakeSingleValue(9));

  // Thursday, March 28, 2019 9:13:00 AM GMT-07:00
  ASSERT_EQ(1553789580, NextTime(expression, start_time));
}

}  // namespace
}  // namespace Cron
}  // namespace eva
