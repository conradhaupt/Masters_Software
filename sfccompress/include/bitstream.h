// ================================================================================================
// -*- C++ -*-
// File: huffman.hpp
// Author: Guilherme R. Lampert
// Created on: 15/02/16
// Brief: Minimal implementation of Huffman encoding/decoding in C++11.
// ================================================================================================

// ================================================================================================
// Taken from the original implementation of Huffman Encoding by Guilherme R.
// Lampert The original license is given below
// ================================================================================================

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

#ifndef LIBSFCC_BITSTREAM_H
#define LIBSFCC_BITSTREAM_H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include "utils.h"

namespace sfcc {
class Code final
{
 public:
  static constexpr int MaxBits = 64;

  Code();

  void clear();

  void appendBit(const int bit);

  void appendCode(const Code other);

  int getBit(const int index) const;

  std::string toBitString() const;

  int getLength() const;
  void setLength(const int lenInBits);

  std::uint64_t getAsU64() const;
  void setAsU64(const std::uint64_t num);

  bool operator==(const Code other) const;
  bool operator!=(const Code other) const;

 private:
  std::uint64_t
      bits;    // The long word storing our code bits, from right to left.
  int length;  // Bit position within the bits word to write next. Same as the
               // current length in bits.
};

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
  void appendCode(Code code);

#ifndef HUFFMAN_NO_STD_STRING
  std::string toBitString() const;  // Useful for debugging.
  void appendBitString(const std::string &bitStr);
#endif  // HUFFMAN_NO_STD_STRING

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

  void reset();
  bool readNextBit();
  std::uint64_t readBitsU64(int bitCount);

  // Basic stream info:
  int getByteCount() const;
  int getBitCount() const;
  const std::uint8_t *getBitStream() const;

  // Current Huffman code being read from the stream:
  void clearCode();
  Code getCode() const;
  int getCodeLength() const;

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
  Code currCode;  // Current Huffman code being built from the input bit stream.
};
};  // namespace sfcc
#endif