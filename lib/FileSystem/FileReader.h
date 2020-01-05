#ifndef EVA_LIB_FILESYSTEM_FILEREADER_
#define EVA_LIB_FILESYSTEM_FILEREADER_

#include <cassert>
#include <optional>
#include <string>

namespace eva::fs {

// A file reader which reads line by line.
class FileReader {
 public:
  // `max_line_len` specifies the max line length. If excceed, triggers a fatal
  // error.
  FileReader(std::string file_name, unsigned int max_line_len = 500)
      : file_name_{std::move(file_name)}, max_line_len_{max_line_len} {
    assert(max_line_len_ > 0);
  }

  // Cleans the resource.
  ~FileReader();

 public:
  // Returns empty optional if end of file.
  std::optional<std::string> nextline();

 private:
  // Loads next block from file.
  void loadNextBuffer();

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
