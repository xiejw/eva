#include "gtest/gtest.h"
#include "lib/Support/Error.h"

namespace {

class FooTest : public ::testing::Test {
};

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}

}  // namespace
