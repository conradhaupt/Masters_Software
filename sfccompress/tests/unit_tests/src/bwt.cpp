/**
 * @file bwt.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "bwt.h"
#include <gtest/gtest.h>
#include "compressor.h"

TEST(bwt, bwtSortsCharsAppropriately)
{
  char data[] = "BANANAICECREAMSUNDAY";
  const int length = 20;
  auto [bwt, first, last] = sfcc::easyBWT<char>(data, length);
  char expected[] = {'Y', 'N', 'E', 'N', 'B', 'D',        //
                     'D',                                 // The EOL
                     'I', 'E', 'N', 'R', 'C', 'A', 'A',   //
                     'A', 'A', 'U', 'C', 'M', 'S', 'A'};  //
  char actual[21];
  for (auto i = 0; i < length; i++) actual[i] = bwt[i];
  const int expectedPosEOL = 6;
  for (auto i = 0; i < length; i++) {
    EXPECT_EQ(bwt[i], expected[i]);
  }
  EXPECT_EQ(last, expectedPosEOL);
}

TEST(bwt, bwtSortsIntsAppropriately)
{
  std::uint8_t data[] = {2, 7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0, 1};
  const int length = 15;
  auto [bwt, first, last] = sfcc::easyBWT<std::uint8_t>(data, length);
  std::uint8_t expected[] = {1, 9, 0, 7, 8, 8, 8, 8, 8, 4, 2, 2, 1, 1, 2, 5};
  char actual[16];
  for (auto i = 0; i < length; i++) actual[i] = bwt[i];
  const int expectedPosEOL = 5;
  for (auto i = 0; i < length; i++) {
    EXPECT_EQ(bwt[i], expected[i]);
  }
  EXPECT_EQ(last, expectedPosEOL);
}

TEST(mtf, MTFOfIncreasingSequenceIsCorrect)
{
  std::uint16_t data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::uint16_t expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto [mtf, min, max] =
      ::sfcc::easyMTFAnyuType<std::uint16_t, std::uint16_t>(data, 10);
  for (auto i = 0; i < 10; i++) EXPECT_EQ(mtf[i], expected[i]);
  EXPECT_EQ(min, 0);
  EXPECT_EQ(max, 9);
}

TEST(bzip, compressesIntegerDataCorrectly)
{
  // uint16 version of e
  std::uint16_t data_raw[] = {65282, 65287, 65281, 65288, 65282,
                              65288, 65281, 65288, 65282, 65288,
                              65284, 65285, 65289, 65280, 65281};
  std::uint16_t expected_bwt[] = {65281, 65289, 65280, 65287, 65288, 65288,
                                  65288, 65288, 65288, 65284, 65282, 65282,
                                  65281, 65281, 65282, 65285};
  std::uint16_t expected[] = {1, 9, 2, 8, 9, 0, 0, 0, 0, 7, 6, 0, 6, 0, 1, 8};
  constexpr int data_size = 15;
  auto data = std::make_unique<std::uint8_t[]>(data_size * 2);
  std::copy((std::uint8_t*)data_raw, (std::uint8_t*)(data_raw + data_size),
            data.get());

  auto [bwt, first, last] =
      ::sfcc::easyBWT((std::uint16_t*)data.get(), data_size);
  auto [mtf, min, max] = ::sfcc::easyMTFAnyuType<std::uint16_t, std::uint16_t>(
      (std::uint16_t*)bwt.get(), data_size + 1);

  // Confirm MTF of BWT result
  for (auto i = 0; i < data_size + 1; i++) {
    EXPECT_EQ(mtf[i], expected[i]);
  }

  // Confirm BWT result
  for (auto i = 0; i < (data_size + 1) * 2; i++) {
    EXPECT_EQ(bwt[i], *((std::uint8_t*)expected_bwt + i));
  }
}