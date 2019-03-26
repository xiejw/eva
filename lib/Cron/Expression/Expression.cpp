#include "lib/Cron/Expression/Expression.h"

namespace eva {
namespace Cron {

bool Expression::Match(time_t time) {
  auto time_tm = std::make_unique<tm>();

  localtime_r(&time, time_tm.get());

  if (!minute_->Match(time_tm->tm_min)) {
    return false;
  }

  if (!hour_->Match(time_tm->tm_hour)) {
    return false;
  }

  if (!day_->Match(time_tm->tm_mday)) {
    return false;
  }

  // tm_wday starts with 0 (Sunday).
  if (!dayOfWeek_->Match(time_tm->tm_wday)) {
    return false;
  }

  // tm_mon starts with zero.
  auto month = time_tm->tm_mon + 1;
  if (!month_->Match(month)) {
    return false;
  }

  return true;
}

}  // namespace Cron
}  // namespace eva
