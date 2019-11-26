#include <iostream>

#include "lib/FileSystem/FileSystem.h"

int main(int argc, char** argv) {
  std::cout << "Hello\n";
  eva::fs::WalkTree(".");
  return 0;
}
