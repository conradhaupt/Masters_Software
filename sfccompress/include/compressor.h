/**
 * @file compressor.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-08-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFCC_COMPRESSOR_H
#define SFCC_COMPRESSOR_H
#include <divsufsort.h>
#include <algorithm>
#include <exception>
#include <memory>
#include <tuple>
#include "bwt.h"
#include "huffman.h"
#include "lz77.h"
#include "lzw.h"
#include "mtf.h"
#include "rle.h"
#include "sfcc.h"
namespace sfcc {

class compressor
{
 public:
  compressor() {}
  // We don't want to copy as std::unique_ptr isn't copyable
  compressor(const compressor& comp) = delete;
  virtual ~compressor() {}

  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length,
           const sfc::sfcc_header& header) = 0;
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length,
             const sfc::sfcc_header& header) = 0;
  virtual std::string getFileExtension() const = 0;
  virtual sfc::sfcc::compression_t getCompressionType() const = 0;
};

class no_compressor
{
 public:
  no_compressor();
  // We don't want to copy as std::unique_ptr isn't copyable
  no_compressor(const compressor& comp) = delete;
  virtual ~no_compressor();

  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;
};

class huffman : public compressor
{
 public:
  huffman();
  virtual ~huffman();

  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;
};

class lz77 : public compressor
{
 public:
  lz77();
  virtual ~lz77();
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;
};

class lzw : public compressor
{
 public:
  lzw();
  virtual ~lzw();
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;
};

class rle : public compressor
{
 public:
  rle();
  virtual ~rle();
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;
};

// class deflate : public compressor
// {
//  public:
//   deflate();
//   virtual ~deflate();
//   virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
//                      sfc::sfcc_header>
//   compress(const std::unique_ptr<std::uint8_t[]>& data,
//            const unsigned long long& length, const sfc::sfcc_header& header);
//   virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
//                      sfc::sfcc_header>
//   decompress(const std::unique_ptr<std::uint8_t[]>& data,
//              const unsigned long long& length, const sfc::sfcc_header&
//              header);
//   virtual std::string getFileExtension() const;
//   virtual sfc::sfcc::compression_t getCompressionType() const;
// };

template <class postcompressor>
class bzip : public compressor
{
 public:
  bzip(bool forceByteCompression = false);
  virtual ~bzip();

  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  compress(const std::unique_ptr<std::uint8_t[]>& data,
           const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
                     sfc::sfcc_header>
  decompress(const std::unique_ptr<std::uint8_t[]>& data,
             const unsigned long long& length, const sfc::sfcc_header& header);
  virtual std::string getFileExtension() const;
  virtual sfc::sfcc::compression_t getCompressionType() const;

 protected:
  const bool _forceByteCompression;
  constexpr static std::uint16_t MAX_BLOCK_SIZE_BYTES_EXP =
      20;  // 2^ceillog2(900k)), close to the maximum block size of BZIP2
  constexpr static std::uint32_t MAX_BLOCK_SIZE_BYTES =
      1U << MAX_BLOCK_SIZE_BYTES_EXP;  // 2^ceil(log2(900k)), close to the
  // maximum block size of BZIP2
  struct Block
  {
    std::uint32_t length;  // Length in bytes
    std::uint8_t* data;
    std::optional<std::int8_t> min_value;
    std::optional<std::int8_t> max_value;
    std::optional<std::uint32_t> bwt_primary_index;
    std::optional<std::uint8_t> npaddingbits;
    bool encoded = false;
    std::uint8_t* dataptr() { return data; }
    // Number of bytes in un/compressed data
    std::uint32_t data_length() { return length; }
    std::uint32_t size()
    {
      std::uint32_t _size{};
      // Block structure
      if (encoded) {
        _size += sizeof(std::uint32_t);  // Block size (excluding metadata)
        if (npaddingbits) _size += sizeof(npaddingbits.value());
        if (min_value) _size += sizeof(min_value.value());
        if (max_value) _size += sizeof(max_value.value());
        if (bwt_primary_index) _size += sizeof(bwt_primary_index.value());
      }
      _size += length;
      return _size;
    }

    std::unique_ptr<std::uint8_t[]> toArray()
    {
      auto array = std::make_unique<std::uint8_t[]>(size());
      auto array_ptr = array.get();
      if (encoded) {
        *((std::uint32_t*)array_ptr) = data_length();
        array_ptr += sizeof(data_length());

        if (npaddingbits) {
          *((std::uint8_t*)array_ptr) = npaddingbits.value();
          array_ptr += sizeof(npaddingbits.value());
        }

        if (min_value) {
          *((std::uint8_t*)array_ptr) = min_value.value();
          array_ptr += sizeof(min_value.value());
        }

        if (max_value) {
          *((std::uint8_t*)array_ptr) = max_value.value();
          array_ptr += sizeof(max_value.value());
        }
        if (bwt_primary_index) {
          *((std::uint32_t*)array_ptr) = bwt_primary_index.value();
          array_ptr += sizeof(bwt_primary_index.value());
        }
      }

      std::copy(data, data + data_length(), array_ptr);

      return std::move(array);
    }
  };
};  // namespace sfcc

std::unique_ptr<compressor> make_compressor(
    const ::sfc::sfcc::compression_t& comp, const bool& bitTransposed = false);
}  // namespace sfcc
#endif
