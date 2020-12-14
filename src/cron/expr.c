#include "cron/expr.h"

typedef int Error;
typedef int bool;
#define kOK      0
#define kFAILURE 1
#define true 1
#define false 0

enum Component { minute, hour, day, dayOfWeek, month };

// Rewinds all components in the candidate up to `up_to`. For example, if
// `up_to` is `day`, then `hour`, `minute` are rewinded to `zero`.
static void rewind(struct tm* candidate, enum Component up_to) {
  switch (up_to) {
    case month:
      candidate->tm_mday = 1;
      // fallthrough
    case dayOfWeek:
      // fallthrough
    case day:
      candidate->tm_hour = 0;
      // fallthrough
    case hour:
      candidate->tm_min = 0;
      break;
    case minute:
      // no-op
      break;
    default:
      assert(0);
      // FatalError("Unexpected component: %d", up_to);
  }
}

/// Validates the candidate. We only search up to 5 years.
static Error validate(const struct tm* const candidate,
                      const struct tm* const start_time) {
  return candidate->tm_year - start_time->tm_year > 5;
}

/// Retrieves the component from the time.
static int value(struct tm* time_tm, enum Component component) {
  switch (component) {
    case minute:
      return time_tm->tm_min;
    case hour:
      return time_tm->tm_hour;
    case day:
      return time_tm->tm_mday;
    case dayOfWeek:
      return time_tm->tm_wday;
    case month:
      // Month starts with 0 in `tm`.
      return time_tm->tm_mon + 1;
    default:
      assert(0);
      // FatalError("Unexpected component: %d", component);
  }
}

// Increases the component of `time_tm` by 1. Adjusted all other components
// accordingly.
//
// For example: if the minute reaches 60, minute will be reset as 0 and hour
// will be increaesed.
static void increase(struct tm* time_tm, enum Component component) {
  switch (component) {
    case minute:
      time_tm->tm_min += 1;
      break;
    case hour:
      time_tm->tm_hour += 1;
      break;
    case day:
      time_tm->tm_mday += 1;
      break;
    case dayOfWeek:
      time_tm->tm_mday += 1;
      break;
    case month:
      time_tm->tm_mon += 1;
      break;
    default:
      assert(0);
      // FatalError("Unexpected component: %d", component);
  }
  mktime(time_tm);
}

// Focusing only on the component inside the `candidate`, searching the next
// value, by keeping increasing it, until matching the `field` of the
// expression.
static Error searchNextMatching(const struct tm* const start_time,
                                cron_field_t*          field,
                                const enum Component   component,
                                struct tm* candidate, bool* changed) {
  int initial_value = value(candidate, component);
  if (cronFieldMatch(field, initial_value)) {
    *changed = false;
    return kOK;
  }

  *changed = true;
  rewind(candidate, component);

  while (true) {
    increase(candidate, component);
    if (validate(candidate, start_time)) {
      return kFAILURE;
    }
    int new_value = value(candidate, component);

    if (cronFieldMatch(field, new_value)) {
      return kOK;
    }
  }
}

int cronExprMatch(cron_expr_t* expr, time_t time) {
  struct tm time_tm;
  localtime_r(&time, &time_tm);

  if (!cronFieldMatch(&expr->minute, time_tm.tm_min)) return false;
  if (!cronFieldMatch(&expr->hour, time_tm.tm_hour)) return false;
  if (!cronFieldMatch(&expr->day, time_tm.tm_mday)) return false;
  // tm_wday starts with 0 (Sunday).
  if (!cronFieldMatch(&expr->day_of_week, time_tm.tm_wday)) return false;
  // tm_mon starts with zero.
  if (!cronFieldMatch(&expr->month, time_tm.tm_mon + 1)) return false;

  return true;
}

/// Algorithm to find next time matching the eva::Cron::Expression.
///
/// Step 1: Go to next minute and reset the second field.
/// Step 2 (mainLoop:) : Search minute until match.
/// Step 3: Search hour until match. If hour changed, reset lower components
///         (minute in this case) and jump to `mainLoop`.
/// Step 4: Search day until match. If day changed, reset lower components
///         (hour and minute in this case) and jump to `mainLoop`.
/// Step 5-6: Search month and year. Same rule as step 3 and 4.
Error cronExprNext(cron_expr_t* expr, time_t start_time, time_t* next_time) {
  struct tm candidate;
  localtime_r(&start_time, &candidate);

  // Make a deep copy.
  struct tm start_point = candidate;

  // Advance the minute once to start the search and reset the second field.
  candidate.tm_min += 1;
  candidate.tm_sec = 0;

mainLoop:
  while (true) {
    for (enum Component component = minute; component <= month; component++) {
      cron_field_t* field;
      switch (component) {
        case minute:
          field = &expr->minute;
          break;
        case hour:
          field = &expr->hour;
          break;
        case day:
          field = &expr->day;
          break;
        case dayOfWeek:
          field = &expr->day_of_week;
          break;
        case month:
          field = &expr->month;
          break;
        default:
          assert(0);
          // FatalError("Unexpected component: %d", component);
      }

      bool changed;
      if (searchNextMatching(/*start_time = */ &start_point,
                             /*field = */ field,
                             /*component = */ component,
                             /*candidate = */ &candidate,
                             /*changed = */ &changed))
        return kFAILURE;

      if (changed) goto mainLoop;
    }
    break;
  }

  *next_time = mktime(&candidate);
  return kOK;
}
