/**
 * @file coordinates.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-12
 *
 * @copyright Copyright (c) 2020
 *
 */
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
TEST(coordinates, greaterThanReturnsTrueIfAllElementsAreGreater)
{
  sfc::coordinates<int, 5> c1{1, 2, 3, 4, 5};
  sfc::coordinates<int, 5> c2{2, 3, 4, 5, 6};
  sfc::coordinates<int, 5> c3{2, 3, 3, 5, 6};
  EXPECT_TRUE(sfc::coords::greaterThan(c2, c1));
  EXPECT_FALSE(sfc::coords::greaterThan(c3, c1));
}

TEST(coordinates, greaterThanEqualsReturnsTrueIfAllElementsAreGreaterOrEqual)
{
  sfc::coordinates<int, 5> c1{1, 2, 3, 4, 5};
  sfc::coordinates<int, 5> c2{2, 3, 4, 5, 6};
  sfc::coordinates<int, 5> c3{2, 3, 3, 5, 6};
  EXPECT_TRUE(sfc::coords::greaterThanEqual(c2, c1));
  EXPECT_TRUE(sfc::coords::greaterThanEqual(c3, c1));
}

TEST(coordinates, lessThanReturnsTrueIfAllElementsAreLesser)
{
  sfc::coordinates<int, 5> c1{1, 2, 3, 4, 5};
  sfc::coordinates<int, 5> c2{2, 3, 4, 5, 6};
  sfc::coordinates<int, 5> c3{2, 3, 3, 5, 6};
  EXPECT_TRUE(sfc::coords::lessThan(c1, c2));
  EXPECT_FALSE(sfc::coords::lessThan(c1, c3));
}

TEST(coordinates, lessThanEqualsReturnsTrueIfAllElementsAreLessOrEqual)
{
  sfc::coordinates<int, 5> c1{1, 2, 3, 4, 5};
  sfc::coordinates<int, 5> c2{2, 3, 4, 5, 6};
  sfc::coordinates<int, 5> c3{2, 3, 3, 5, 6};
  EXPECT_TRUE(sfc::coords::lessThanEqual(c1, c2));
  EXPECT_TRUE(sfc::coords::lessThanEqual(c1, c3));
}
};  // namespace