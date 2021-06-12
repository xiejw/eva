#include "testing/testing.h"

#include <time.h>

#include "cron/expr.h"

#define ASSERT_NO_ERR(expr) ASSERT_TRUE("no error.", 0 == (expr))

static char *
test_minute()
{
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

static char *
test_hour()
{
        // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
        time_t      current_time = 1553711518;
        time_t      next_time;
        cron_expr_t expr;
        cronExprInit(&expr);
        cronFieldSetSingleValue(&expr.hour, 13);
        ASSERT_NO_ERR(cronExprNext(&expr, current_time, &next_time));
        // Wednesday, March 27, 2019 1:00:00 PM GMT-07:00
        ASSERT_TRUE("next time mismatch.", 1553716800 == next_time);
        return NULL;
}

static char *
test_hour_in_next_day()
{
        // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
        time_t      current_time = 1553711518;
        time_t      next_time;
        cron_expr_t expr;
        cronExprInit(&expr);
        cronFieldSetSingleValue(&expr.hour, 9);
        ASSERT_NO_ERR(cronExprNext(&expr, current_time, &next_time));
        // Thursday, March 28, 2019 9:00:00 AM GMT-07:00
        ASSERT_TRUE("next time mismatch.", 1553788800 == next_time);
        return NULL;
}

static char *
test_end_to_end()
{
        // Wednesday, March 27, 2019 11:31:58 AM GMT-07:00 DST
        time_t      current_time = 1553711518;
        time_t      next_time;
        cron_expr_t expr;
        cronExprInit(&expr);
        cronFieldSetSingleValue(&expr.hour, 9);
        cronFieldSetSingleValue(&expr.minute, 13);
        ASSERT_NO_ERR(cronExprNext(&expr, current_time, &next_time));
        // Thursday, March 28, 2019 9:13:00 AM GMT-07:00
        ASSERT_TRUE("next time mismatch.", 1553789580 == next_time);
        return NULL;
}

char *
run_cron_suite()
{
        char *tz_value = getenv("TZ");
        setenv("TZ", "America/Los_Angeles", /*overwrite = */ 1);

        RUN_TEST(test_minute);
        RUN_TEST(test_hour);
        RUN_TEST(test_hour_in_next_day);
        RUN_TEST(test_end_to_end);

        if (tz_value)
                setenv("TZ", tz_value, /*overwrite = */ 1);
        else
                unsetenv("TZ");

        return NULL;
}
