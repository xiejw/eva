#ifndef CRON_EXPR_H_
#define CRON_EXPR_H_

#include <time.h>

#include "base/defs.h"
#include "cron/field.h"

typedef struct {
        cron_field_t minute;
        cron_field_t hour;
        cron_field_t day;
        cron_field_t month;
        cron_field_t day_of_week;
} cron_expr_t;

extern void    cronExprInit(cron_expr_t *expr);
extern error_t cronExprNext(cron_expr_t *expr, time_t start_time,
                            _mut_ time_t *next_time);
extern int     cronExprMatch(cron_expr_t *expr, time_t time);

#endif
