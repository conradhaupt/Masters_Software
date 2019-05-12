#include "libsfc/iterator.h"
#include <gtest/gtest.h>
#include "simple_curve.h"

namespace {
using iterator = sfc::iterator<2>;
TEST(iterator, isCopyConstructibleUsingConstructor)
{
  simplecurve crv(16);
  iterator it_control(crv);
  iterator it_1(crv);
  auto it_2(it_1);
  // Check if the assignment was done correctly
  EXPECT_EQ(it_1, it_2);
  // Check if the original iterator was modified
  EXPECT_EQ(it_1, it_control);
}

TEST(iterator, isCopyConstructibleUsingAssigment)
{
  simplecurve crv(16);
  iterator it_control(crv);
  iterator it_1(crv);
  auto it_2 = it_1;
  // Check if the assignment was done correctly
  EXPECT_EQ(it_1, it_2);
  // Check if the original iterator was modified
  EXPECT_EQ(it_1, it_control);
}

TEST(iterator, typetraits_is_copy_constructible)
{
  EXPECT_TRUE(std::is_copy_constructible_v<iterator>);
}

TEST(iterator, typetraits_is_copy_assignable)
{
  EXPECT_TRUE(std::is_copy_assignable_v<iterator>);
}

TEST(iterator, typetraits_is_destructible)
{
  EXPECT_TRUE(std::is_destructible_v<iterator>);
}

TEST(iterator, typetraits_is_swappable)
{
  EXPECT_TRUE(std::is_swappable_v<iterator>);
}

TEST(iterator, outOfBoundsBasedOnDistanceBounds)
{
  simplecurve crv(16);
  auto it = std::begin(crv);
  for (; it != std::end(crv); it++) {
    EXPECT_EQ((it.distance() <= crv.maxDistance()), !it.outofbounds());
  }
  EXPECT_EQ((it.distance() > crv.maxDistance()), it.outofbounds());
}

TEST(iterator, notOutOfBoundsBeforeEnd)
{
  simplecurve crv(16);
  auto it = std::begin(crv);
  for (; it != std::end(crv); it++) {
    EXPECT_FALSE(it.outofbounds());
  }
}

TEST(iterator, outOfBoundsAtEnd)
{
  simplecurve crv(16);
  auto end = std::end(crv);
  EXPECT_TRUE(end.outofbounds());
}

TEST(iterator, dereferenceNoThrowBeforeEnd)
{
  simplecurve crv(16);
  for (auto it = std::begin(crv); it != std::end(crv); it++) {
    EXPECT_NO_THROW(auto i = *it);
  }
}

TEST(iterator, dereferenceAtEndThrowsOutOfBoundsException)
{
  simplecurve crv(16);
  auto end = std::end(crv);
  EXPECT_THROW(auto i = *end, sfc::OutOfBoundsException);
}

TEST(iterator, stdBeginIsBegin)
{
  simplecurve crv(16);
  auto begin = std::begin(crv);
  EXPECT_EQ(begin.distance(), simplecurve::dist_type{});
}

TEST(iterator, stdEndIsEnd)
{
  simplecurve crv(16);
  auto end = std::end(crv);
  EXPECT_TRUE(end.outofbounds());
  EXPECT_TRUE(end.distance() > crv.maxDistance());
}

TEST(iterator, coordinatesMatchCurveCoordinatesForGivenDistance)
{
  simplecurve crv(16);
  for (auto p : crv) {
    auto dist = p.distance;
    auto coords = p.coords;
    EXPECT_EQ(coords, crv.distToCoords(dist));
  }
}

};  // namespace
