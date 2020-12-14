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

extern void cronExprInit(cron_expr_t* expr);
extern int  cronExprNext(cron_expr_t* expr, time_t start_time,
                         time_t* next_time);
extern int  cronExprMatch(cron_expr_t* expr, time_t time);

#endif
