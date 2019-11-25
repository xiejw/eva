#include <ctime>

#include "lib/Cron/Expression/Expression.h"
#include "lib/Support/Error.h"

namespace eva {
namespace Cron {

namespace {

enum Component { minute, hour, day, dayOfWeek, month };

/// Rewinds all components in the candidate up to `up_to`. For example, if
/// `up_to` is `day`, then `hour`, `minute` are rewinded to `zero`.
void rewind(tm* candidate, Component up_to) {
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
      FatalError("Unexpected component: %d", up_to);
  }
}

/// Validates the candidate. We only search up to 5 years.
Error validate(const tm* const candidate, const tm* const start_time) {
  return candidate->tm_year - start_time->tm_year > 5;
}

/// Retrieves the component from the time.
int value(tm* time_tm, Component component) {
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
      FatalError("Unexpected component: %d", component);
  }
}

// Increases the component of `time_tm` by 1. Adjusted all other components
// accordingly.
//
// For example: if the minute reaches 60, minute will be reset as 0 and hour
// will be increaesed.
void increase(tm* time_tm, Component component) {
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
      FatalError("Unexpected component: %d", component);
  }
  mktime(time_tm);
}

// Focusing only on the component inside the `candidate`, searching the next
// value, by keeping increasing it, until matching the `field` of the
// expression.
Error searchNextMatching(const tm* const start_time, const Field* const field,
                         const Component component, tm* candidate,
                         bool* changed) {
  auto initial_value = value(candidate, component);
  if (field->Match(initial_value)) {
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
    auto new_value = value(candidate, component);

    if (field->Match(new_value)) {
      return kOK;
    }
  }
}

}  // namespace

bool Expression::Match(Time time) {
  auto time_tm = std::make_unique<tm>();
  localtime_r(&time, time_tm.get());

  if (!minute_->Match(time_tm->tm_min)) return false;
  if (!hour_->Match(time_tm->tm_hour)) return false;
  if (!day_->Match(time_tm->tm_mday)) return false;
  // tm_wday starts with 0 (Sunday).
  if (!dayOfWeek_->Match(time_tm->tm_wday)) return false;
  // tm_mon starts with zero.
  if (!month_->Match(time_tm->tm_mon + 1)) return false;

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
Error Expression::Next(Time start_time, Time* next_time) {
  tm candidate;
  localtime_r(&start_time, &candidate);

  // Make a deep copy.
  tm start_point{candidate};

  // Advance the minute once to start the search and reset the second field.
  candidate.tm_min += 1;
  candidate.tm_sec = 0;

mainLoop:
  while (true) {
    for (auto component : {minute, hour, day, dayOfWeek, month}) {
      Field* field = nullptr;
      switch (component) {
        case minute:
          field = minute_.get();
          break;
        case hour:
          field = hour_.get();
          break;
        case day:
          field = day_.get();
          break;
        case dayOfWeek:
          field = dayOfWeek_.get();
          break;
        case month:
          field = month_.get();
          break;
        default:
          FatalError("Unexpected component: %d", component);
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

}  // namespace Cron
}  // namespace eva
