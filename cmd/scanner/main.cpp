#include <iostream>

#include "lib/FileSystem/FileSystem.h"

void callback(const eva::fs::WalkStat& stat) {
  if (stat.is_folder)
    std::cout << "@\t" << stat.path.c_str() << "\n";
  else
    std::cout << stat.size << "\t" << stat.path.c_str() << "\n";
}

int main(int argc, char** argv) {
  eva::fs::WalkTree(".", callback);
  return 0;
}
