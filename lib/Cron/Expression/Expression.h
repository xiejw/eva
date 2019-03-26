#ifndef LIB_CRON_EXPRESSION_EXPRESSION
#define LIB_CRON_EXPRESSION_EXPRESSION

#include "lib/Cron/Expression/Field.h"

namespace eva {
namespace Cron {

class Expression {
 public:
  Expression(Field minute) : minute_(minute) {}

 private:
  Field minute_;
};

}  // namespace Cron
}  // namespace eva

#endif
