// Written as a simple header file with all code so the main.cpp can include
// this.
//
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "eva/FileSystem/FileReader.h"
#include "eva/FileSystem/Glob.h"
#include "eva/Foundation/Macros.h"
#include "eva/Foundation/Status.h"

namespace {

struct Record {
  std::string checksum;

  // The path to the file having this checksum.
  std::string checksum_path;

  // The file the record is stored in.
  std::string file_path;
};

class Database {
 public:
  Database(std::string pattern) : pattern_{std::move(pattern)} {};

 public:
  eva::Status refresh();

  std::pair<std::list<const Record>*, bool> lookup(const std::string& key);

 private:
  // Restores the records into the database.
  eva::Status record_file(const std::string& file_path);

 private:
  std::string pattern_;
  std::unordered_map<std::string, std::list<const Record>> map_ = {};
};

[[nodiscard]] std::pair<std::list<const Record>*, bool> Database::lookup(
    const std::string& key) {
  const auto result = map_.find(key);
  return (result == map_.end()) ? std::make_pair(nullptr, false)
                                : std::make_pair(&result->second, true);
}

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
  while (true) {
    EVA_ASSIGN_OR_RETURN(textline, r.nextline());
    if (textline.end_of_file) break;

    // Make sure the line is somehow valid.
    auto line = textline.content;
    auto pos = line.find_first_of(' ');
    if (pos == std::string::npos) continue;
    EVA_CHECK(pos > 0 && pos < line.size());

    // Decomp the line.
    Record r;
    r.checksum = line.substr(0, pos);
    r.checksum_path = line.substr(pos + 1);
    r.file_path = file_path;
    map_[r.checksum].push_back(std::move(r));

    count++;
  }
  std::cout << "    Line Count: " << count << "\n";
  std::cout << "    DB Count: " << map_.size() << "\n";
  return eva::Status::OK;
};

}  // namespace
