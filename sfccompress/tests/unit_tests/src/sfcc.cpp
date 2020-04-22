/**
 * @file sfcc.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "sfcc.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <type_traits>

namespace {
sfc::sfcc::sfc_t sfc_types[] = {
    sfc::sfcc::sfc_t::ROW_MAJOR, sfc::sfcc::sfc_t::SNAKE_SCAN,
    sfc::sfcc::sfc_t::MORTON, sfc::sfcc::sfc_t::GRAY_CODE,
    sfc::sfcc::sfc_t::HILBERT};
TEST(sfc_t, doNotConflictInUInt8Conversion)
{
  // Compare all combinations
  for (auto v : sfc_types) {
    for (auto u : sfc_types) {
      // Ignore the same enum class values
      if (u != v) {
        EXPECT_NE(std::uint8_t(v), std::uint8_t(u));
      }
    }
  }
}

TEST(sfc_t, rowMajorIsZeroUInt8)
{
  EXPECT_EQ(std::uint8_t(sfc::sfcc::sfc_t::ROW_MAJOR), std::uint8_t(0));
}

TEST(sfc_t, snakeScanIsOneUInt8)
{
  EXPECT_EQ(std::uint8_t(sfc::sfcc::sfc_t::SNAKE_SCAN), std::uint8_t(1));
}

TEST(sfc_t, mortonIsTwoUInt8)
{
  EXPECT_EQ(std::uint8_t(sfc::sfcc::sfc_t::MORTON), std::uint8_t(2));
}

TEST(sfc_t, grayCodeIsThreeUInt8)
{
  EXPECT_EQ(std::uint8_t(sfc::sfcc::sfc_t::GRAY_CODE), std::uint8_t(3));
}

TEST(sfc_t, hilbertIsFourUInt8)
{
  EXPECT_EQ(std::uint8_t(sfc::sfcc::sfc_t::HILBERT), std::uint8_t(4));
}
}  // namespace
