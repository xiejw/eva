#ifndef EVA_FOUNDATION_STRINGS_H_
#define EVA_FOUNDATION_STRINGS_H_

#include <sstream>

namespace eva {

class Strings {
 public:
  template <typename... Args>
  inline static std::string concat(const Args&... args) {
    std::stringstream ss;
    concat_impl(ss, args...);
    return ss.str();
  };

 protected:
  template <typename T>
  inline static void concat_impl(std::stringstream& ss, const T& t) {
    ss << t;
  };

  template <typename T, typename... Args>
  inline static void concat_impl(std::stringstream& ss, const T& t,
                                 const Args&... args) {
    ss << t;
    concat_impl(ss, args...);
  };
};

}  // namespace eva

#endif
