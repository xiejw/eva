
TEST_F(CronExpressionTest, CheckMinute) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto expression = Expression(/*minute = */ Field::MakeSingleValue(40));

  // Wednesday, March 27, 2019 11:40:00 AM GMT-07:00
  ASSERT_EQ(1553712000, NextTime(expression, start_time));
}

TEST_F(CronExpressionTest, CheckHour) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto         expression = Expression(/*minute = */ Field::MakeAny(),
                               /*hour = */ Field::MakeSingleValue(13));

  // Wednesday, March 27, 2019 1:00:00 PM GMT-07:00
  ASSERT_EQ(1553716800, NextTime(expression, start_time));
}

TEST_F(CronExpressionTest, CheckHourInNextDay) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto         expression = Expression(/*minute = */ Field::MakeAny(),
                               /*hour = */ Field::MakeSingleValue(9));

  // Thursday, March 28, 2019 9:00:00 AM GMT-07:00
  ASSERT_EQ(1553788800, NextTime(expression, start_time));
}

TEST_F(CronExpressionTest, EndToEnd) {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  const time_t start_time = 1553711518;
  auto         expression = Expression(/*minute = */ Field::MakeSingleValue(13),
                               /*hour = */ Field::MakeSingleValue(9));

  // Thursday, March 28, 2019 9:13:00 AM GMT-07:00
  ASSERT_EQ(1553789580, NextTime(expression, start_time));
}
