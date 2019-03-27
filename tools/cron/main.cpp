#include <gflags/gflags.h>
#include <ctime>

#include <iostream>
#include <memory>

#include "lib/Cron/Expression/Expression.h"
#include "lib/Cron/Expression/Field.h"

DEFINE_int32(minute, -1, "The minute field. `-1` means `*`.");
DEFINE_int32(hour, -1, "The hour field. `-1` means `*`.");

std::unique_ptr<eva::Cron::Field> ReadFromFlag(
    const gflags::int32& flag_value) {
  if (flag_value == -1) return eva::Cron::Field::MakeAny();

  return eva::Cron::Field::MakeSingleValue(flag_value);
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  auto current_time = time(nullptr);
  auto expression = eva::Cron::Expression(
      /* minute =*/ReadFromFlag(FLAGS_minute),
      /* hour =*/ReadFromFlag(FLAGS_hour));

  time_t next_time;
  if (expression.Next(current_time, &next_time)) {
    // Error happened.
    exit(1);
  }
  std::cout << (next_time - current_time);
  return 0;
}
