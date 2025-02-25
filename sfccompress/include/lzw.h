
// ================================================================================================
// -*- C++ -*-
// File: lzw.hpp
// Author: Guilherme R. Lampert
// Created on: 17/02/16
// Brief: LZW encoder/decoder in C++11 with varying length dictionary codes.
// ================================================================================================

#ifndef LZW_HPP
#define LZW_HPP

// ---------
//  LICENSE
// ---------
// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy,
// distribute, and modify this file as you see fit.
//
// The source code is provided "as is", without warranty of any kind, express or
// implied. No attribution is required, but a mention about the author is
// appreciated.
//
// -------
//  SETUP
// -------
// #define LZW_IMPLEMENTATION in one source file before including
// this file, then use lzw.hpp as a normal header file elsewhere.
//
// ----------
//  OVERVIEW
// ----------
// Lempel–Ziv–Welch (LZW) encoder/decoder.
//
// This is the compression scheme used by the GIF image format and the Unix
// 'compress' tool. Main differences from this implementation is that End Of
// Input (EOI) and Clear Codes (CC) are not stored in the output and the max
// code length in bits is 12, vs 16 in compress.
//
// EOI is simply detected by the end of the data stream, while CC happens if the
// dictionary gets filled. Data is written/read from bit streams, which handle
// byte-alignment for us in a transparent way.
//
// The decoder relies on the hardcoded data layout produced by the encoder,
// since no additional reconstruction data is added to the output, so they must
// match. The nice thing about LZW is that we can reconstruct the dictionary
// directly from the stream of codes generated by the encoder, so this avoids
// storing additional headers in the bit stream.
//
// The output code length is variable. It starts with the minimum number of bits
// required to store the base byte-sized dictionary and automatically increases
// as the dictionary gets larger (it starts at 9-bits and grows to 10-bits when
// code 512 is added, then 11-bits when 1024 is added, and so on). If the
// dictionary is filled (4096 items for a 12-bits dictionary), the whole thing
// is cleared and the process starts over. This is the main reason why the
// encoder and the decoder must match perfectly, since the lengths of the codes
// will not be specified with the data itself.
//
// --------------
//  USEFUL LINKS
// --------------
// https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch
// http://rosettacode.org/wiki/LZW_compression
// http://www.cs.duke.edu/csed/curious/compression/lzw.html
// http://www.cs.cf.ac.uk/Dave/Multimedia/node214.html
// http://marknelson.us/1989/10/01/lzw-data-compression/

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <tuple>

// Disable the bit stream => std::string dumping methods.
#ifndef LZW_NO_STD_STRING
#include <string>
#endif  // LZW_NO_STD_STRING

// If you provide a custom malloc(), you must also provide a custom free().
// Note: We never check LZW_MALLOC's return for null. A custom implementation
// should just abort with a fatal error if the program runs out of memory.
#ifndef LZW_MALLOC
#define LZW_MALLOC std::malloc
#define LZW_MFREE std::free
#endif  // LZW_MALLOC

namespace lzw {

// ========================================================

// The default fatalError() function writes to stderr and aborts.
#ifndef LZW_ERROR
void fatalError(const char *message);
#define LZW_USING_DEFAULT_ERROR_HANDLER
#define LZW_ERROR(message) ::lzw::fatalError(message)
#endif  // LZW_ERROR

// ========================================================
// class BitStreamWriter:
// ========================================================

class BitStreamWriter final
{
 public:
  // No copy/assignment.
  BitStreamWriter(const BitStreamWriter &) = delete;
  BitStreamWriter &operator=(const BitStreamWriter &) = delete;

  BitStreamWriter();
  explicit BitStreamWriter(int initialSizeInBits, int growthGranularity = 2);

  void allocate(int bitsWanted);
  void setGranularity(int growthGranularity);
  std::uint8_t *release();

  void appendBit(int bit);
  void appendBitsU64(std::uint64_t num, int bitCount);

#ifndef LZW_NO_STD_STRING
  std::string toBitString() const;  // Useful for debugging.
  void appendBitString(const std::string &bitStr);
#endif  // LZW_NO_STD_STRING

  int getByteCount() const;
  int getBitCount() const;
  const std::uint8_t *getBitStream() const;

  ~BitStreamWriter();

 private:
  void internalInit();
  static std::uint8_t *allocBytes(int bytesWanted, std::uint8_t *oldPtr,
                                  int oldSize);

  std::uint8_t *stream;  // Growable buffer to store our bits. Heap allocated &
                         // owned by the class instance.
  int bytesAllocated;    // Current size of heap-allocated stream buffer *in
                         // bytes*.
  int granularity;  // Amount bytesAllocated multiplies by when auto-resizing in
                    // appendBit().
  int currBytePos;  // Current byte being written to, from 0 to
                    // bytesAllocated-1.
  int nextBitPos;   // Bit position within the current byte to access next. 0
                    // to 7.
  int numBitsWritten;  // Number of bits in use from the stream buffer, not
                       // including byte-rounding padding.
};

// ========================================================
// class BitStreamReader:
// ========================================================

class BitStreamReader final
{
 public:
  // No copy/assignment.
  BitStreamReader(const BitStreamReader &) = delete;
  BitStreamReader &operator=(const BitStreamReader &) = delete;

  BitStreamReader(const BitStreamWriter &bitStreamWriter);
  BitStreamReader(const std::uint8_t *bitStream, int byteCount, int bitCount);

  bool isEndOfStream() const;
  bool readNextBit(int &bitOut);
  std::uint64_t readBitsU64(int bitCount);
  void reset();

 private:
  const std::uint8_t
      *stream;  // Pointer to the external bit stream. Not owned by the reader.
  const int
      sizeInBytes;  // Size of the stream *in bytes*. Might include padding.
  const int sizeInBits;  // Size of the stream *in bits*, padding *not* include.
  int currBytePos;       // Current byte being read in the stream.
  int nextBitPos;   // Bit position within the current byte to access next. 0
                    // to 7.
  int numBitsRead;  // Total bits read from the stream so far. Never includes
                    // byte-rounding padding.
};

// ========================================================
// LZW Dictionary helper:
// ========================================================
constexpr int Nil = -1;
constexpr int MaxDictBits = 12;
constexpr int StartBits = 9;
constexpr int FirstCode = (1 << (StartBits - 1));   // 256
constexpr int MaxDictEntries = (1 << MaxDictBits);  // 4096

bool entries_compare(const std::tuple<int, int> &lhs,
                     const std::tuple<int, int> &rhs);

class Dictionary final
{
 public:
  struct Entry
  {
    int code;
    int value;
  };

  // Dictionary entries 0-255 are always reserved to the byte/ASCII range.
  int size;
  Entry entries[MaxDictEntries];
  std::map<std::tuple<int, int>, int> entries_map;

  Dictionary();
  int findIndex(int code, int value) const;
  bool add(int code, int value);
  bool flush(int &codeBitsWidth);
};

// ========================================================
// easyEncode() / easyDecode():
// ========================================================

// Quick LZW data compression. Output compressed data is heap allocated
// with LZW_MALLOC() and should be later freed with LZW_MFREE().
void easyEncode(const std::uint8_t *uncompressed, int uncompressedSizeBytes,
                std::uint8_t **compressed, int *compressedSizeBytes,
                int *compressedSizeBits);

// Decompress back the output of easyEncode().
// The uncompressed output buffer is assumed to be big enough to hold the
// uncompressed data, if it happens to be smaller, the decoder will return a
// partial output and the return value of this function will be less than
// uncompressedSizeBytes.
int easyDecode(const std::uint8_t *compressed, int compressedSizeBytes,
               int compressedSizeBits, std::uint8_t *uncompressed,
               int uncompressedSizeBytes);

}  // namespace lzw

// ================== End of header file ==================
#endif  // LZW_HPP
        // ================== End of header file ==================