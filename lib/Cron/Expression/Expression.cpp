#include <ctime>

#include "lib/Cron/Expression/Expression.h"

namespace eva {
namespace Cron {

namespace {

using Time = time_t;

enum Component { minute, hour, day, dayOfWeek, month };

/// Rewinds all components in the candidate up to `upTo`. For example, if
/// `upTo` is `.day`, then `.hour`, `.minute`, and `.second` are rewinded to
/// zero.
void rewind(tm* candidate, Component up_to) {
  switch (up_to) {
    case month:
      candidate->tm_mday = 1;
      // fallthrough
    case dayOfWeek:
    case day:
      candidate->tm_hour = 0;
      // fallthrough
    case hour:
      candidate->tm_min = 0;
      break;
    default:
      // FIXME: falal error.
      exit(1);
  }
}


// Returns true if invalid.
bool validate(tm* candidate, tm* start_time) {
  // We only search up to 5 years.
  return candidate->tm_year - start_time->tm_year > 5;
}

int value(tm* time_tm, Component component) {
      switch (component) {
        case month: return time_tm->tm_mon + 1;
        case dayOfWeek: return time_tm->tm_wday;
        case day: return time_tm->tm_mday;
        case hour:return time_tm->tm_hour;
        case minute:return time_tm->tm_min;
        default:
          // FIXME: falal error.
          exit(1);
      }
}

void
increase(tm* time_tm, Component component) {
      switch (component) {
        case month: time_tm->tm_mon += 1; break;
        case dayOfWeek: time_tm->tm_mday += 1; break;
        case day:time_tm->tm_mday += 1; break;
        case hour:time_tm->tm_hour += 1; break;
        case minute:time_tm->tm_min += 1; break;
        default:
          // FIXME: falal error.
          exit(1);
      }
 auto new_time = mktime(time_tm);
 localtime_r(&new_time, time_tm);
}

// Returns false if invalid.
bool searchNextMatching(tm* start_time, Field* field, tm* time_tm, Component component, bool* changed) {
  auto initial_value = value(time_tm, component);
  if (field->Match(initial_value)) {
    *changed = false;
    return false;
  }

  *changed = true;
  rewind(time_tm, component);

  while (true) {
    increase(time_tm, component);
    if (validate(time_tm, start_time)) {
      return true;
    }
    auto new_value = value(time_tm, component);

    if (field->Match(new_value)) {
      return false;
    }
  }
}


}  // namespace anonymouse

bool Expression::Match(Time time) {
  auto time_tm = std::make_unique<tm>();
  localtime_r(&time, time_tm.get());

  if (!minute_->Match(time_tm->tm_min))
    return false;

  if (!hour_->Match(time_tm->tm_hour))
    return false;

  if (!day_->Match(time_tm->tm_mday))
    return false;

  // tm_wday starts with 0 (Sunday).
  if (!dayOfWeek_->Match(time_tm->tm_wday))
    return false;

  // tm_mon starts with zero.
  if (!month_->Match(time_tm->tm_mon + 1))
    return false;

  return true;
}

bool Expression::Next(Time start_time, Time *next_time) {
  auto time_tm = std::make_unique<tm>();
  localtime_r(&start_time, time_tm.get());

  // Make a deep copy.
  tm start_point {*time_tm};

  time_tm->tm_min += 1;
  time_tm->tm_sec = 0;

mainLoop:
  while (true) {
    for (auto component: {minute, hour, day, dayOfWeek, month}) {
      Field* field = nullptr;
      switch (component) {
        case month: field = month_.get(); break;
        case dayOfWeek: field = dayOfWeek_.get(); break;
        case day: field = day_.get(); break;
        case hour: field = hour_.get(); break;
        case minute: field = minute_.get(); break;
        default:
          // FIXME: falal error.
          exit(1);
      }

      bool changed;
      if (searchNextMatching(&start_point, field, time_tm.get(), component, &changed)) {
        return true;
      }
      if (changed) {
        goto mainLoop;
      }
    }
    break;
  }

  *next_time = mktime(time_tm.get());
  return false;
}

}  // namespace Cron
}  // namespace eva
