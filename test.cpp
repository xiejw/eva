#include <ctime>
#include <iostream>
#include <memory>

#include "lib/Cron/Expression/Field.h"

int main() {
  auto current_time = time(nullptr);
  auto result = std::make_unique<tm>();

  localtime_r(&current_time, result.get());

  std::cout << "hello\n";
  std::cout << "Current epoch is " << current_time << std::endl;
  std::cout << "Year: " << result->tm_year + 1970 << std::endl;
  std::cout << "Month: " << result->tm_mon + 1 << std::endl;
  std::cout << "Day: " << result->tm_mday << std::endl;
  std::cout << "Hour: " << result->tm_hour << std::endl;
  std::cout << "Minute: " << result->tm_min << std::endl;
  std::cout << "Second: " << result->tm_sec << std::endl;
}
