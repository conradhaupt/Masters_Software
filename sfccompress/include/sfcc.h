/**
 * @file sfcc.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-08-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef LIBSFCC_SFCC_H
#define LIBSFCC_SFCC_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace sfc {

const bool DEBUG = false;

namespace sfcc {

enum class sfc_t : std::uint8_t
{
  ROW_MAJOR = 0,
  SNAKE_SCAN = 1,
  MORTON = 2,
  GRAY_CODE = 3,
  HILBERT = 4
};

extern std::unordered_map<std::string, sfc_t> sfc_string_t;

enum class compression_t : std::uint8_t
{
  NONE = 0,
  RLE = 1,
  HUFFMAN = 2,
  LZ77 = 3,
  LZ78 = 4,
  LZW = 5,
  DEFLATE = 6,
  BZIP_LZ77 = 7,
  BZIP_LZW = 8,
  BWT = 9
};

extern std::unordered_map<std::string, compression_t> compression_string_t;

extern const std::string magicword;
extern const std::uint8_t magicword_uint8_t[];

static std::string tostring(const sfc_t& sfctype)
{
  for (auto [str, sfc] : sfc_string_t)
    if (sfc == sfctype) return str;
  return "NULL";
}

static std::string tostring(const compression_t& compressiontype)
{
  for (auto [str, comp] : compression_string_t)
    if (comp == compressiontype) return str;
  return "NULL";
}
}  // namespace sfcc

struct sfcc_header
{
  std::uint8_t ndims;
  std::uint8_t sidelength_k;
  sfcc::sfc_t sfctype;
  sfcc::compression_t compressiontype;
  std::uint8_t dtype_nbytes;
  std::optional<std::uint8_t> npaddingbits;
  bool bittransposed;
  std::optional<std::uint32_t> output_block_length;

  static bool CompressionRequiresPaddingBits(
      const sfcc::compression_t& compressionType)
  {
    switch (compressionType) {
      case sfcc::compression_t::HUFFMAN:
      case sfcc::compression_t::LZW:
      case sfcc::compression_t::DEFLATE:
      case sfcc::compression_t::BZIP_LZW:
        return true;
      default:
        return false;
    }
  }

  int size() const
  {
    auto _size = 9;
    if (npaddingbits && CompressionRequiresPaddingBits(compressiontype))
      _size++;
    return _size;
  }
};

std::unique_ptr<std::uint8_t[]> HeaderToArray(const sfcc_header& header);

class sfcc_file
{
 public:
  std::uint8_t HEADER_MAGIC[4] = {'S', 'F', 'C', 'C'};

 private:
  sfcc_header _header;
  std::unique_ptr<std::uint8_t[]> _data_file;
  std::future<std::unique_ptr<std::uint8_t[]>> _data;
  unsigned long long _data_size;

  std::unique_ptr<std::uint8_t[]> loadData(std::shared_ptr<std::ifstream> file,
                                           unsigned long long data_size);

  inline void _dataWaitAndGet()
  {
    if (!_data_file) {
      _data_file = _data.get();
    }
  }

 public:
  sfcc_file(const std::string& filename);
  // ~sfcc_file(){}
  sfcc_header& getHeader() { return _header; }
  const sfcc_header& getHeader() const { return _header; }
  std::uint8_t* getData()
  {
    _dataWaitAndGet();
    return _data_file.get();
  }
  const std::unique_ptr<std::uint8_t[]>& getDataPointer()
  {
    _dataWaitAndGet();
    return _data_file;
  }
  unsigned long long size() const { return _data_size; }
  unsigned long long sizein_bytes() const { return _data_size; }
};

}  // namespace sfc

#endif