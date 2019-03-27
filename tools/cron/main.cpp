#include <gflags/gflags.h>
#include <ctime>

#include <iostream>
#include <memory>

#include "lib/Cron/Expression/Expression.h"
#include "lib/Cron/Expression/Field.h"

DEFINE_int32(minute, -1, "The minute field. `-1` means `*`.");
DEFINE_int32(hour, -1, "The hour field. `-1` means `*`.");

void PrintTime(const time_t& current_time) {
  auto result = std::make_unique<tm>();

  localtime_r(&current_time, result.get());

  std::cout << "Current epoch is " << current_time << std::endl;
  std::cout << "Year: " << result->tm_year + 1900 << std::endl;
  std::cout << "Month: " << result->tm_mon + 1 << std::endl;
  std::cout << "Day of week: " << result->tm_wday << std::endl;
  std::cout << "Day: " << result->tm_mday << std::endl;
  std::cout << "Hour: " << result->tm_hour << std::endl;
  std::cout << "Minute: " << result->tm_min << std::endl;
  std::cout << "Second: " << result->tm_sec << std::endl;
}

std::unique_ptr<eva::Cron::Field> ReadFromFlag(
    const gflags::int32& flag_value) {
  if (flag_value == -1) return eva::Cron::Field::MakeAny();

  return eva::Cron::Field::MakeSingleValue(flag_value);
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  auto current_time = time(nullptr);
  PrintTime(current_time);

  auto expression = eva::Cron::Expression(
      /* minute =*/ReadFromFlag(FLAGS_minute),
      /* hour =*/ReadFromFlag(FLAGS_hour));

  time_t next_time;
  expression.Next(current_time, &next_time);
  std::cout << "Next: " << std::endl;
  PrintTime(next_time);
}
