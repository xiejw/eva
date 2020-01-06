// Written as a simple header file with all code so the main.cpp can include
// this.
//
#include <memory>
#include <string>
#include <unordered_map>

#include "eva/FileSystem/FileReader.h"
#include "lib/FileSystem/Glob.h"

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

void Database::refresh() {
  std::cout << "Pattern: " << pattern_ << "\n";

  eva::fs::Glob g{pattern_};
  g.refresh();
  assert(g.results().size() > 0);

  for (auto& file : g.results()) {
    std::cout << "  -> File: " << file << "\n";
    eva::fs::FileReader r{file};
    int count = 0;
    while (!r.nextline().end_of_file) {
      count++;
    }
    std::cout << "    Line Count: " << count << "\n";
  }
};

}  // namespace
