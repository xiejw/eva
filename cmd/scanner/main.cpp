#include <iostream>

#include "lib/Crypto/Crypto.h"
#include "lib/FileSystem/FileSystem.h"

void callback(const eva::fs::WalkStat& stat) {
  if (stat.is_folder)
    std::cout << "@\t" << stat.path.c_str() << "\n";
  else
    std::cout << stat.size << "\t" << stat.path.c_str() << "\n";
}

int main(int argc, char** argv) {
  eva::fs::WalkTree(".", callback);
  std::string input = "grape";
  std::string output1 = eva::crypto::Sha256(input);

  std::cout << "sha256('" << input << "'):" << output1 << std::endl;
  std::cout << "sha256('"
            << "\"\""
            << "'):" << eva::crypto::Sha256("") << std::endl;
  return 0;
}
