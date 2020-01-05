#ifndef EVA_LIB_FILESYSTEM_FILEREADER_
#define EVA_LIB_FILESYSTEM_FILEREADER_

#include <optional>
#include <string>

namespace eva::fs {

// A file reader which reads line by line.
class FileReader {
 public:
  FileReader(std::string file_name) : file_name_{std::move(file_name)} {}
  ~FileReader();

 public:
  // Returns emtpy optional if end of file.
  std::optional<std::string> nextline();

 private:
  void loadNextBuffer();

 private:
  std::string file_name_;

  int fd_ = -1;
  std::unique_ptr<unsigned char[]> buffer_ = {};
  ssize_t pos_ = 0;
  ssize_t allocated_ = 0;
  bool end_of_file_ = false;
};

}  // namespace eva::fs

#endif
