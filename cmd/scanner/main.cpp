#include <iostream>

#include <gflags/gflags.h>

#include "cmd/scanner/Database.h"
#include "lib/FileSystem/FileSystem.h"

DEFINE_string(dir, ".", "The directory to scan.");
DEFINE_int32(max_num_files, -1, "The nax number of files to scan.");

constexpr bool kFetchChecksum = true;

int main(int argc, char** argv) {
  // Sets up gflags.
  gflags::SetUsageMessage("scanner");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Sets up the max_num_files.
  std::optional<int> max_num_files{};
  if (FLAGS_max_num_files > 0) max_num_files = FLAGS_max_num_files;

  eva::fs::FileTree tree{FLAGS_dir.c_str(), {kFetchChecksum, max_num_files}};
  tree.Refresh();

  for (auto& handle : tree) {
    if (kFetchChecksum)
      std::cout << handle->checksum.value() << " " << handle->path << "\n";
    else
      std::cout << handle->size << "\t" << handle->path << "\n";
  }
  return 0;
}
