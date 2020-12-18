#include "testing/testing.h"

#include <time.h>

#include "cron/expr.h"

#define ASSERT_NO_ERR(expr) ASSERT_TRUE("no error.", 0 == (expr))

static char* test_cron_minute() {
  // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
  time_t      current_time = 1553711518;
  time_t      next_time;
  cron_expr_t expr;
  cronExprInit(&expr);
  cronFieldSetSingleValue(&expr.minute, 40);
  ASSERT_NO_ERR(cronExprNext(&expr, current_time, &next_time));
  // Wednesday, March 27, 2019 11:40:00 AM GMT-07:00
  ASSERT_TRUE("next time mismatch.", 1553712000 == next_time);
  return NULL;
}

char* run_cron_suite() {
  char* tz_value = getenv("TZ");
  setenv("TZ", "America/Los_Angeles", /*overwrite = */ 1);

  RUN_TEST(test_cron_minute);

  if (tz_value)
    setenv("TZ", tz_value, /*overwrite = */ 1);
  else
    unsetenv("TZ");

  return NULL;
}
