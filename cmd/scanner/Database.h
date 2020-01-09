// Written as a simple header file with all code so the main.cpp can include
// this.
//
#include <memory>
#include <string>
#include <unordered_map>

#include "eva/FileSystem/FileReader.h"
#include "eva/FileSystem/Glob.h"
#include "eva/Foundation/Status.h"

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
  eva::Status refresh();

 private:
  // Restores the records into the database.
  eva::Status record_file(const std::string& file_path);

 private:
  std::string pattern_;
};

[[nodiscard]] eva::Status Database::refresh() {
  std::cout << "Pattern: " << pattern_ << "\n";

  eva::fs::Glob g{pattern_};
  g.refresh();
  assert(g.results().size() > 0);

  for (auto& file : g.results()) {
    std::cout << "  -> File: " << file << "\n";
    EVA_RETURN_IF_ERROR(record_file(file));
  }
  return eva::Status::OK;
};

[[nodiscard]] eva::Status Database::record_file(const std::string& file_path) {
  eva::fs::FileReader r{file_path};
  int count = 0;
  while (!r.nextline().consumeValue().end_of_file) {
    count++;
  }
  std::cout << "    Line Count: " << count << "\n";
  return eva::Status::OK;
};

}  // namespace
