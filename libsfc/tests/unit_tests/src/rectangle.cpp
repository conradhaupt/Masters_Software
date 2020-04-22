/**
 * @file rectangle.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "libsfc/rectangle.h"
#include <gtest/gtest.h>

namespace {
TEST(rectangle, mergesConstructorPointsProperly)
{
  sfc::coordinates<int, 5> c1{1, 1, 1, 7, 7};
  sfc::coordinates<int, 5> c2{7, 7, 7, 1, 1};
  sfc::coordinates<int, 5> expectMin{1, 1, 1, 1, 1};
  sfc::coordinates<int, 5> expectMax{7, 7, 7, 7, 7};
  sfc::rectangle<int, 5> r(c1, c2);
  EXPECT_EQ(expectMin, r.minCoord());
  EXPECT_EQ(expectMax, r.maxCoord());
}

TEST(rectangle, containsPointsWithinBounds)
{
  sfc::coordinates<int, 5> c1{1, 1, 1, 1, 1};
  sfc::coordinates<int, 5> c2{7, 7, 7, 7, 7};
  sfc::rectangle<int, 5> r(c1, c2);
  sfc::coordinates<int, 5> coordWithin{2, 3, 4, 5, 6};
  EXPECT_TRUE(r.contains(coordWithin));
}
TEST(rectangle, doesNotContainPointsOutsideOfBounds)
{
  sfc::coordinates<int, 5> c1{1, 1, 1, 1, 1};
  sfc::coordinates<int, 5> c2{7, 7, 7, 7, 7};
  sfc::rectangle<int, 5> r(c1, c2);
  sfc::coordinates<int, 5> coordOutside{0, 3, 4, 8, 6};
  EXPECT_FALSE(r.contains(coordOutside));
}
TEST(rectangle, areaReturnsProductOfSideLengths)
{
  sfc::coordinates<int, 5> c1{1, 1, 1, 1, 1};
  sfc::coordinates<int, 5> c2{7, 7, 7, 7, 7};
  sfc::rectangle<int, 5> r(c1, c2);
  auto expectedArea = 7 * 7 * 7 * 7 * 7;
  EXPECT_EQ(expectedArea, r.area());
}
}  // namespace
