
#include <gtest/gtest.h>

#include "mindmap/utils.hpp"

TEST(dummy, gtest_dot_to_png) {
  using namespace mm;
  ASSERT_TRUE(
      to_png(TEST_ROOT "test_mindmap.dot", TEST_ROOT "test_mindmap.png"));
}
