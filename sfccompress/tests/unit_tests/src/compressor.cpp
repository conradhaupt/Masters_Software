#include "compressor.h"
#include <gtest/gtest.h>
#include "compressor_data.h"

TEST_P(CompressorDataTestFicture, huffmanIsReversible)
{
  auto compressor = ::sfcc::huffman();
  auto [compressed_data, compressed_length, compressed_header] =
      compressor.compress(file.getDataPointer(), file.size(), file.getHeader());
  auto [decompressed_data, decompressed_length, decompressed_header] =
      compressor.decompress(compressed_data, compressed_length,
                            compressed_header);

  EXPECT_EQ(expectedSize, decompressed_length);
  const auto original_data = file.getData();
  for (auto i = 0; i < expectedSize; i++)
    EXPECT_EQ(original_data[i], decompressed_data[i]);
}

TEST_P(CompressorDataTestFicture, lz77IsReversible)
{
  auto compressor = ::sfcc::lz77();
  auto [compressed_data, compressed_length, compressed_header] =
      compressor.compress(file.getDataPointer(), file.size(), file.getHeader());
  auto [decompressed_data, decompressed_length, decompressed_header] =
      compressor.decompress(compressed_data, compressed_length,
                            compressed_header);

  EXPECT_EQ(expectedSize, decompressed_length);
  const auto original_data = file.getData();
  for (auto i = 0; i < expectedSize; i++)
    EXPECT_EQ(original_data[i], decompressed_data[i]);
}

TEST_P(CompressorDataTestFicture, lzwIsReversible)
{
  auto compressor = ::sfcc::lzw();
  auto [compressed_data, compressed_length, compressed_header] =
      compressor.compress(file.getDataPointer(), file.size(), file.getHeader());
  auto [decompressed_data, decompressed_length, decompressed_header] =
      compressor.decompress(compressed_data, compressed_length,
                            compressed_header);

  EXPECT_EQ(expectedSize, decompressed_length);
  const auto original_data = file.getData();
  for (auto i = 0; i < expectedSize; i++)
    EXPECT_EQ(original_data[i], decompressed_data[i]);
}

TEST_P(CompressorDataTestFicture, noCompressorIsReversible)
{
  auto compressor = ::sfcc::no_compressor();
  auto [compressed_data, compressed_length, compressed_header] =
      compressor.compress(file.getDataPointer(), file.size(), file.getHeader());
  auto [decompressed_data, decompressed_length, decompressed_header] =
      compressor.decompress(compressed_data, compressed_length,
                            compressed_header);

  EXPECT_EQ(expectedSize, decompressed_length);
  const auto original_data = file.getData();
  for (auto i = 0; i < expectedSize; i++)
    EXPECT_EQ(original_data[i], decompressed_data[i]);
}

TEST_P(CompressorDataTestFicture, rleIsReversible)
{
  auto compressor = ::sfcc::rle();
  auto [compressed_data, compressed_length, compressed_header] =
      compressor.compress(file.getDataPointer(), file.size(), file.getHeader());
  auto [decompressed_data, decompressed_length, decompressed_header] =
      compressor.decompress(compressed_data, compressed_length,
                            compressed_header);

  EXPECT_EQ(expectedSize, decompressed_length);
  const auto original_data = file.getData();
  for (auto i = 0; i < expectedSize; i++)
    EXPECT_EQ(original_data[i], decompressed_data[i]);
}

INSTANTIATE_TEST_CASE_P(Compressor, CompressorDataTestFicture,
                        ::testing::Values(std::make_tuple<std::string, int>(
                            TEST_COMPRESSOR_DATA_FILENAME, 262144 * 2)));