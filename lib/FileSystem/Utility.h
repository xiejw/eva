#ifndef EVA_LIB_FILESYSTEM_UTILITY
#define EVA_LIB_FILESYSTEM_UTILITY

#include <string>

namespace eva {
namespace fs {

// Joins `d_name` with `f_name` as `d_name/f_name`.
//
// - If `d_name` is empty string, returns string{f_name}.
// - If `f_name` is empty string returns string{d_name}.
std::string PathJoin(const char* d_name, const char* f_name);

}  // namespace fs
}  // namespace eva

#endif
