/**
 * @file mtf.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 20219-10-14
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "mtf.h"
#include <gtest/gtest.h>

TEST(mtf, returnsCorrectSequence)
{
  // Data with the alphabet A-G
  char data[] = {'E', 'F', 'G', 'A', 'C', 'A', 'A', 'A', 'G', 'E'};
  const auto length = 10;
  std::uint8_t expected[] = {4, 5, 6, 3, 5, 1, 0, 0, 2, 4};
  auto [mtf, min, max] =
      sfcc::easyMTFAnyuType<char, std::uint8_t>(data, length);
  for (auto i = 0; i < length; i++) EXPECT_EQ(mtf[i], expected[i]);
}