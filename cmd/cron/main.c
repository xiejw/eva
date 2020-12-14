#include <time.h>
#include <stdio.h>

#include "cron/expr.h"

// DEFINE_int32(minute, -1, "The minute field. `-1` means `*`.");
// DEFINE_int32(hour, -1, "The hour field. `-1` means `*`.");

// std::unique_ptr<eva::cron::Field> ReadFromFlag(
//     const gflags::int32& flag_value) {
//   if (flag_value == -1) return eva::cron::Field::MakeAny();
//
//   return eva::cron::Field::MakeSingleValue(flag_value);
// }

// Prints the number of seconds from `now` to the next time matching
// eva::cron::Expression.
int main(int argc, char** argv) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);

  time_t current_time = time(NULL);
  time_t next_time;

  cron_expr_t expr;
  cronExprInit(&expr);
  // Error happened.
  if (cronExprNext(&expr, current_time, &next_time)) exit(1);

  printf("%ld", (long)(next_time - current_time));
  return 0;
}
