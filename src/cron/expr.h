#ifndef CRON_EXPR_H_
#define CRON_EXPR_H_

#include <time.h>

#include "cron/field.h"

typedef struct {
  cron_field_t minute;
  cron_field_t hour;
  cron_field_t day;
  cron_field_t month;
  cron_field_t day_of_week;
} cron_expr_t;

//   using Time = time_t;
//
//
//   // Finds next matching time, filled in `next_time`, starting from
//   // `start_time`.
//   Error Next(Time start_time, Time *next_time);
//
//  private:
//   // Tests whether `time` matches the expression.

int cronExprMatch(cron_expr_t* expr, time_t time);

#endif
