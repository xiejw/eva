#ifndef EVA_FILESYSTEM_FILEREADER_H_
#define EVA_FILESYSTEM_FILEREADER_H_

#include <memory>
#include <string>

#include "eva/Foundation/Macros.h"
#include "eva/Foundation/Status.h"

namespace eva::fs {

// A file reader which reads line by line.
class FileReader {
 public:
  // `max_line_len` specifies the max line length. If excceed, triggers a fatal
  // error.
  FileReader(std::string file_name, unsigned int max_line_len = 500)
      : file_name_{std::move(file_name)}, max_line_len_{max_line_len} {
    EVA_CHECK(max_line_len_ > 0);
  }

  // Cleans the resource.
  ~FileReader();

 public:
  // The result of `nextline`.
  struct TextLine {
    // Indicates whether we reach end of file.
    bool end_of_file;

    // Valid iff `end_of_file` == false.
    std::string line;
  };

  TextLine nextline();

 private:
  // Loads next block from file.
  Status loadNextBuffer();

 private:
  std::string file_name_;
  const unsigned int max_line_len_;

  int fd_ = -1;
  std::unique_ptr<unsigned char[]> buffer_ = {};
  ssize_t pos_ = 0;
  ssize_t allocated_ = 0;
  bool end_of_file_ = false;
};

}  // namespace eva::fs

#endif
