#include "eva/FileSystem/FileReader.h"

#include <iostream>
#include <errno.h>
#include <fcntl.h>  // open
#include <string.h>
#include <unistd.h>

#include "lib/Support/Error.h"

namespace eva::fs {

namespace {

// Byte size to read file block.
constexpr unsigned int kBufferSize = 4096 * 16;

}  // namespace

void FileReader::loadNextBuffer() {
  pos_ = 0;
  allocated_ = read(fd_, (void *)(buffer_.get()), kBufferSize);

  if (allocated_ == -1)
    eva::FatalError("Faied to read file for %s: %s", file_name_.c_str(),
                    strerror(errno));
}

FileReader::~FileReader() {
  if (fd_ != -1 && close(fd_) == -1)
    eva::FatalError("Faied to close file for %s: %s", file_name_.c_str(),
                    strerror(errno));

  fd_ = -1;
}

std::optional<std::string> FileReader::nextline() {
  if (end_of_file_) return {};

  // Initialize all states, if applicable.
  if (!buffer_) {
    fd_ = open(file_name_.c_str(), O_RDONLY);
    if (fd_ == -1)
      eva::FatalError("Faied to open file for %s: %s", file_name_.c_str(),
                      strerror(errno));

    buffer_.reset(new unsigned char[kBufferSize]);
    pos_ = 0;
    allocated_ = 0;
  }

  // Allocates the buffer for current line.
  unsigned char line[max_line_len_];
  int current_len = 0;

  // The loop ends in any of the following condition.
  // 1. end of file.
  // 2. find a end-of-line.
  // 3. reach maximul line length limit.
  while (true) {
    if (pos_ >= allocated_) loadNextBuffer();

    // Checks whether it is end-of-file.
    if (allocated_ == 0) {
      end_of_file_ = true;
      if (current_len == 0) {
        // If there is nothing left after the last end-of-line, we should not
        // produce this.
        return {};
      }
      return std::string((const char *)line, current_len);
    }

    // Tries to find end-of-line, i.e., `\n`
    assert(pos_ < allocated_);
    for (auto i = pos_; i < allocated_; ++i) {
      if (buffer_[i] == '\n') {
        int length = i - pos_;
        if (current_len + length >= max_line_len_) {
          eva::FatalError("Line length is too long.");
        }
        memcpy((void *)(line + current_len), (void *)(buffer_.get() + pos_),
               length);
        current_len += length;
        pos_ = i + 1;
        return std::string((const char *)line, current_len);
      }
    }

    // We reach the end of the buffer.
    int length = allocated_ - pos_;
    if (current_len + length >= max_line_len_) {
      eva::FatalError("Line length is too long.");
    }
    memcpy((void *)(line + current_len), (void *)(buffer_.get() + pos_),
           length);
    current_len += length;
    pos_ = allocated_;
    // next iteration.
  }
}

}  // namespace eva::fs
