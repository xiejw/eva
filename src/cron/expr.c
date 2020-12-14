#include "cron/expr.h"

typedef struct tm tm_t;

typedef enum {
  cron_minute,
  cron_hour,
  cron_day,
  cron_day_of_week,
  cron_month,
  cron_final_component_marker
} cron_comp_t;

// -----------------------------------------------------------------------------
// helper methods prototypes.
// -----------------------------------------------------------------------------

static void rewind(tm_t* candidate, cron_comp_t up_to);
static int  validate(tm_t* candidate, tm_t* start_time);
static int  value(tm_t* time_tm, cron_comp_t comp);
static void increase(tm_t* time_tm, cron_comp_t comp);
static int  searchNextMatching(tm_t* start_time, cron_field_t* field,
                               cron_comp_t comp, tm_t* candidate, int* changed);

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

void cronExprInit(cron_expr_t* expr) {
  assert(expr != NULL);
  cronFieldSetAsAny(&expr->minute);
  cronFieldSetAsAny(&expr->hour);
  cronFieldSetAsAny(&expr->day);
  cronFieldSetAsAny(&expr->month);
  cronFieldSetAsAny(&expr->day_of_week);
}

int cronExprMatch(cron_expr_t* expr, time_t time) {
  tm_t time_tm;
  localtime_r(&time, &time_tm);

  if (!cronFieldMatch(&expr->minute, time_tm.tm_min)) return 0;
  if (!cronFieldMatch(&expr->hour, time_tm.tm_hour)) return 0;
  if (!cronFieldMatch(&expr->day, time_tm.tm_mday)) return 0;
  // tm_wday starts with 0 (Sunday).
  if (!cronFieldMatch(&expr->day_of_week, time_tm.tm_wday)) return 0;
  // tm_mon starts with zero.
  if (!cronFieldMatch(&expr->month, time_tm.tm_mon + 1)) return 0;

  return 1;
}

// Algorithm to find next time matching the `expr`
//
// Step 1:   Go to next minute and reset the second field.
// Step 2    (mainLoop:) : Search minute until match.
// Step 3:   Search hour until match. If hour changed, reset lower components
//           (minute in this case) and jump to `mainLoop`.
// Step 4:   Search day until match. If day changed, reset lower components
//           (hour and minute in this case) and jump to `mainLoop`.
// Step 5-6: Search month and year. Same rule as step 3 and 4.
int cronExprNext(cron_expr_t* expr, time_t start_time, time_t* next_time) {
  tm_t candidate;
  localtime_r(&start_time, &candidate);
  tm_t start_point = candidate;  // make a copy.

  // Advance the minute once to start the search and reset the second field.
  candidate.tm_min += 1;
  candidate.tm_sec = 0;

mainLoop:
  while (1) {
    for (cron_comp_t comp = cron_minute; comp < cron_final_component_marker;
         comp++) {
      cron_field_t* field;
      switch (comp) {
        case cron_minute:
          field = &expr->minute;
          break;
        case cron_hour:
          field = &expr->hour;
          break;
        case cron_day:
          field = &expr->day;
          break;
        case cron_day_of_week:
          field = &expr->day_of_week;
          break;
        case cron_month:
          field = &expr->month;
          break;
        default:
          assert(0);
          // FatalError("Unexpected component: %d", component);
      }

      int changed;
      if (searchNextMatching(/*start_time=*/&start_point,
                             /*field=*/field,
                             /*comp=*/comp,
                             /*candidate=*/&candidate,
                             /*changed=*/&changed))
        return -1;

      if (changed) goto mainLoop;
    }
    break;
  }

  *next_time = mktime(&candidate);
  return 0;
}

// -----------------------------------------------------------------------------
// helper methods. implementation.
// -----------------------------------------------------------------------------

// Rewinds all components in the candidate up to `up_to`. For example, if
// `up_to` is `day`, then `hour`, `minute` are rewinded to `zero`.
void rewind(tm_t* candidate, cron_comp_t up_to) {
  switch (up_to) {
    case cron_month:
      candidate->tm_mday = 1;
      // fallthrough
    case cron_day_of_week:
      // fallthrough
    case cron_day:
      candidate->tm_hour = 0;
      // fallthrough
    case cron_hour:
      candidate->tm_min = 0;
      break;
    case cron_minute:
      // no-op
      break;
    default:
      assert(0);
      // FatalError("Unexpected comp: %d", up_to);
  }
}

// Validates the candidate. We only search up to 5 years.
int validate(tm_t* candidate, tm_t* start_time) {
  return candidate->tm_year - start_time->tm_year > 5;
}

// Retrieves the component from the time.
int value(tm_t* time_tm, cron_comp_t comp) {
  switch (comp) {
    case cron_minute:
      return time_tm->tm_min;
    case cron_hour:
      return time_tm->tm_hour;
    case cron_day:
      return time_tm->tm_mday;
    case cron_day_of_week:
      return time_tm->tm_wday;
    case cron_month:
      // Month starts with 0 in `tm_t`.
      return time_tm->tm_mon + 1;
    default:
      assert(0);
      // FatalError("Unexpected comp: %d", comp);
  }
}

// Increases the component of `time_tm` by 1. Adjusted all other components
// accordingly.
//
// For example: if the minute reaches 60, minute will be reset as 0 and hour
// will be increaesed.
void increase(tm_t* time_tm, cron_comp_t comp) {
  switch (comp) {
    case cron_minute:
      time_tm->tm_min += 1;
      break;
    case cron_hour:
      time_tm->tm_hour += 1;
      break;
    case cron_day:
      time_tm->tm_mday += 1;
      break;
    case cron_day_of_week:
      time_tm->tm_mday += 1;
      break;
    case cron_month:
      time_tm->tm_mon += 1;
      break;
    default:
      assert(0);
      // FatalError("Unexpected comp: %d", comp);
  }
  mktime(time_tm);
}

// Focusing only on the comp inside the `candidate`, searching the next
// value, by keeping increasing it, until matching the `field` of the
// expression.
int searchNextMatching(tm_t* start_time, cron_field_t* field, cron_comp_t comp,
                       tm_t* candidate, int* changed) {
  int initial_value = value(candidate, comp);
  if (cronFieldMatch(field, initial_value)) {
    *changed = 0;
    return 0;
  }

  *changed = 1;
  rewind(candidate, comp);

  while (1) {
    increase(candidate, comp);
    if (validate(candidate, start_time)) {
      return -1;
    }
    int new_value = value(candidate, comp);

    if (cronFieldMatch(field, new_value)) {
      return 0;
    }
  }
}
