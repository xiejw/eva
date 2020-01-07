#ifndef EVA_FOUNDATION_STRINGS_H_
#define EVA_FOUNDATION_STRINGS_H_

#include <sstream>

namespace eva {

class Strings {
 public:
  template <class A>
  static std::string concat(const A& a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
  };

  template <class A, class B>
  static std::string concat(const A& a, const B& b) {
    std::stringstream ss;
    ss << a << b;
    return ss.str();
  };

  template <class A, class B, class C>
  static std::string concat(const A& a, const B& b, const C& c) {
    std::stringstream ss;
    ss << a << b << c;
    return ss.str();
  };
};

}  // namespace eva

#endif
