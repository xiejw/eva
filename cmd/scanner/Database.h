// Written as a simple header file with all code so the main.cpp can include
// this.
//
#include <memory>
#include <string>
#include <unordered_map>

namespace {

struct Record {
  std::string checksum;
  std::string checksum_path;
  std::string file_path;
};

class Database {
 public:
  Database(std::string pattern) : pattern_{std::move(pattern)} {};

 public:
  void refresh();

 private:
  std::string pattern_;
};

void Database::refresh() { std::cout << "Pattern: " << pattern_ << "\n"; };

}  // namespace
