#include "libsfc/coordinates.h"
#include <gtest/gtest.h>

namespace {
TEST(coordinates, additionSumsElements)
{
  auto c1 = sfc::coordinates<int, 5>{1, 2, 3, 4, 5};
  auto c2 = sfc::coordinates<int, 5>{2, 3, 4, 5, 6};
  auto expected_c = sfc::coordinates<int, 5>{3, 5, 7, 9, 11};
  EXPECT_EQ((c1 + c2), expected_c);
}

TEST(coordinates, additionAssignmentSumsElements)
{
  auto c1 = sfc::coordinates<int, 5>{1, 2, 3, 4, 5};
  auto c2 = sfc::coordinates<int, 5>{2, 3, 4, 5, 6};
  c1 += c2;
  auto expected_c = sfc::coordinates<int, 5>{3, 5, 7, 9, 11};
  EXPECT_EQ(c1, expected_c);
}
};  // namespace