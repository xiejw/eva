#include <iostream>
#include <string>

#include "lib/Crypto/Sha256.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Need one argument for file.\n";
    return 1;
  }

  std::string digest{};
  if (eva::crypto::SHA256::DigestForFile(argv[1], digest)) {
    std::cout << "Error during shasum evaluation..\n";
    return 3;
  }
  std::cout << digest << "\n";
}
