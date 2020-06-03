/**
 * @file lzw.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "lzw.h"
#include <gtest/gtest.h>
#include <array>
#include <cstdlib>

TEST(lzw, lzwIsReversible)
{
  constexpr auto size = 4096ULL;
  auto data = std::make_unique<std::uint8_t[]>(size);
  // Randomise data
  std::generate(data.get(), data.get() + size,
                []() { return std::uint8_t(std::rand()); });
  std::uint8_t* compressed_temp = nullptr;
  int compressed_bytes = 0;
  int compressed_bits = 0;
  EXPECT_NO_THROW(::lzw::easyEncode(data.get(), size, &compressed_temp,
                                    &compressed_bytes, &compressed_bits));
  std::uint8_t* decompressed_temp = new std::uint8_t[size];
  EXPECT_NO_THROW(::lzw::easyDecode(compressed_temp, compressed_bytes,
                                    compressed_bits, decompressed_temp, size));

  for (auto i = 0ULL; i < size; i++) EXPECT_EQ(data[i], decompressed_temp[i]);
  delete[] decompressed_temp;
  if (compressed_temp != nullptr) delete[] compressed_temp;
}