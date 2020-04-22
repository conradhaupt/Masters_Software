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
      18;  // 2^ceillog2(900k)), close to the maximum block size of BZIP2
  constexpr static std::uint32_t MAX_BLOCK_SIZE_BYTES =
      1U << MAX_BLOCK_SIZE_BYTES_EXP;  // 2^ceil(log2(900k)), close to the
  // maximum block size of BZIP2
  struct Block
  {
    std::uint32_t length;  // Length in bytes
    std::uint8_t* data;
    bool uncompressed = true;
    std::optional<std::uint64_t> min_value;
    std::optional<std::uint64_t> max_value;
    std::optional<std::uint32_t> bwt_iFirst;
    std::optional<std::uint32_t> bwt_iLast;
    std::optional<std::uint8_t> npaddingbits;
    std::uint8_t* dataptr() { return data; }
    std::uint16_t* dataptr_16() { return (std::uint16_t*)data; }
    std::uint32_t* dataptr_32() { return (std::uint32_t*)data; }

    int sizeinbytes(int dtype_nbytes)
    {
      auto size = sizeof(length);
      if (!uncompressed) {
        size += dtype_nbytes * 2;  // Number of bytes for the min and max value
        size += sizeof(bwt_iFirst.value());
        size += sizeof(bwt_iLast.value());
      }
      if (npaddingbits) size += sizeof(npaddingbits.value());
      return size;
    }

    std::uint32_t length_8() { return length; }
    std::uint32_t length_16() { return length / sizeof(std::uint16_t); }
    std::uint32_t length_32() { return length / sizeof(std::uint32_t); }
    std::uint32_t length_var(int dtype_nbytes)
    {
      if (dtype_nbytes == 1)
        return length_8();
      else if (dtype_nbytes == 2)
        return length_16();
      else if (dtype_nbytes == 4)
        return length_32();
      else
        return 0;
    }

    std::unique_ptr<std::uint8_t[]> toArray(int dtype_nbytes)
    {
      auto array = std::make_unique<std::uint8_t[]>(sizeinbytes(dtype_nbytes));
      auto array_ptr = array.get();
      *((std::uint32_t*)array_ptr) = length;
      array_ptr += sizeof(length);

      if (!uncompressed) {
        if (dtype_nbytes == 2) {
          *((std::uint16_t*)array_ptr) = min_value.value();
          array_ptr += dtype_nbytes;
          *((std::uint16_t*)array_ptr) = max_value.value();
          array_ptr += dtype_nbytes;
        } else if (dtype_nbytes == 4) {
          *((std::uint32_t*)array_ptr) = min_value.value();
          array_ptr += dtype_nbytes;
          *((std::uint32_t*)array_ptr) = max_value.value();
          array_ptr += dtype_nbytes;
        } else {
          throw std::runtime_error("Unexpected data-type size");
        }
        *((std::uint32_t*)array_ptr) = bwt_iFirst.value();
        array_ptr += sizeof(bwt_iFirst.value());
        *((std::uint32_t*)array_ptr) = bwt_iLast.value();
        array_ptr += sizeof(bwt_iLast.value());
      }
      return std::move(array);
    }
  };
};  // namespace sfcc

std::unique_ptr<compressor> make_compressor(
    const ::sfc::sfcc::compression_t& comp, const bool& bitTransposed = false);
}  // namespace sfcc
#endif
