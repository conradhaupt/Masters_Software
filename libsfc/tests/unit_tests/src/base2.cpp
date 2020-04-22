/**
 * @file base2.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-10
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "libsfc/base2.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace {
// isPowerOfTwo
TEST(isPowerOfTwo, trueIfPowerOfTwo)
{
  for (unsigned int i : {1, 2, 3, 4, 5, 6, 7, 8}) {
    EXPECT_TRUE(sfc::isPowerOfTwo(1U << i));
  }
}

TEST(isPowerOfTwo, falseIfNotPowerOfTwo)
{
  for (auto i : {2, 3, 4, 5, 6, 7, 8}) {
    EXPECT_FALSE(sfc::isPowerOfTwo((1U << i) - 1));
    EXPECT_FALSE(sfc::isPowerOfTwo((1U << i) + 1));
  }
}

// ilog2
TEST(ilog2, returnsExponentOfBaseTwoValues)
{
  for (auto i : {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    auto num = 1 << i;
    EXPECT_EQ(i, sfc::ilog2(num));
  }
}

TEST(ilog2, throwsIfNotBaseTwo)
{
  for (auto i : {2, 3, 4, 5, 6, 7, 8, 9}) {
    auto num = (1 << i) - 1;
    if (!sfc::isPowerOfTwo(num)) {
      EXPECT_THROW(sfc::ilog2(num), std::invalid_argument);
    }
  }
}
};  // namespace
