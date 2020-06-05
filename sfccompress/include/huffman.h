
// ================================================================================================
// -*- C++ -*-
// File: huffman.hpp
// Author: Guilherme R. Lampert
// Created on: 15/02/16
// Brief: Minimal implementation of Huffman encoding/decoding in C++11.
// ================================================================================================

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

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
// -------------
//  QUICK SETUP
// -------------
// In *one* C++ source file, *before* including this file, do this:
//
//   #define HUFFMAN_IMPLEMENTATION
//
// To enable the implementation. Further includes of this
// file *should not* redefine HUFFMAN_IMPLEMENTATION.
// Example:
//
// In my_program.cpp:
//
//   #define HUFFMAN_IMPLEMENTATION
//   #include "huffman.hpp"
//
// In my_program.hpp:
//
//   #include "huffman.hpp"
//
// ----------
//  OVERVIEW
// ----------
// As-simple-as-possible Huffman encoding and decoding.
//
// The huffman::Encoder and huffman::Decoder classes do
// all the Huffman related work, the rest is just support
// code for bit streams and integer bit manipulation.
//
// easyEncode()/easyDecode() functions are provided
// for quick-n'-easy compression/decompression of raw data
// buffers.
//
// The size of a Huffman code is limited to 64 bits (to fit
// inside a uint64) and no additional handing is done if this
// size is overflowed. It will just log an error and ignore
// further bits.
//
// Symbols are byte-sized so that we can limit the number of leaf
// nodes to 256. There is an extra of 512 nodes for inner nodes,
// but again if that size is exceeded we just log an error and
// ignore.
//
// You can override the HUFFMAN_ERROR() macro to supply your
// own error handling strategy. The default simply writes to
// stderr and calls std::abort().
//
// Memory allocated explicitly by the bit streams will be sourced
// from HUFFMAN_MALLOC/HUFFMAN_MFREE, so you can override the macros
// to add custom memory management. Current that's all the memory
// we allocate directly, but we also use std::priority_queue<> to
// build the Huffman tree and the queue will allocate memory from
// the global heap, so you won't be able to catch that, unfortunately.
//
// The huffman::Node struct is not very optimized for size.
// We use full signed integers for the value and child indexes,
// while the 32-bits range is not strictly needed. All we need
// is the ability to set them to -1 (used as a sentinel value),
// so a signed short would do for the value and child links.
// If you're interested in optimizing for memory usage/size,
// start from there.
//
// The same could also probably be done for huffman::Code.
// If you're sure you'll never need more than 32 bits for
// a code, you could replace the code long-word by a uint32.
// The accompanying code length is an int, which is also overkill,
// since it will never be > 64 (or 32), so if you care about
// saving space in the structure members, you might replace it
// with a byte or short and pack the structure.
//
// --------------
//  USEFUL LINKS
// --------------
// Wikipedia:
//  https://en.wikipedia.org/wiki/Huffman_coding
//
// Nice quick video tutorial on Huffman coding:
//  https://youtu.be/apcCVfXfcqE

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

// Disable the bit stream => std::string dumping methods.
// #ifndef HUFFMAN_NO_STD_STRING
#include <string>
// #endif // HUFFMAN_NO_STD_STRING

// If you provide a custom malloc(), you must also provide a custom free().
// Note: We never check HUFFMAN_MALLOC's return for null. A custom
// implementation should just abort with a fatal error if the program runs out
// of memory.
#ifndef HUFFMAN_MALLOC
#define HUFFMAN_MALLOC std::malloc
#define HUFFMAN_MFREE std::free
#endif  // HUFFMAN_MALLOC

namespace huffman {

// ========================================================

// The default fatalError() function writes to stderr and aborts.
#ifndef HUFFMAN_ERROR
void fatalError(const char *message);
#define HUFFMAN_USING_DEFAULT_ERROR_HANDLER
#define HUFFMAN_ERROR(message) ::huffman::fatalError(message)
#endif  // HUFFMAN_ERROR

// ========================================================
// class Code:
// ========================================================

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
  bool operator<(const Code other) const;

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

// ========================================================
// Huffman Tree Node:
// ========================================================

constexpr int Nil = -1;
constexpr int MaxSymbols = 256;
constexpr int MaxNodes = MaxSymbols + 512;

struct Node final
{
  int frequency = Nil;   // Occurrence count; Nil if not in use.
  int leftChild = Nil;   // Left  gets code 0 assigned to it; Nil initially
  int rightChild = Nil;  // Right gets code 1 assigned to it; Nil initially.
  int value = Nil;       // Symbol value of this node. Interpreted as a byte.
  Code code;             // Huffman code that will be assigned to this node.

  bool isValid() const;
  bool isLeaf() const;
};

// ========================================================
// Huffman encoder class:
// ========================================================

class Encoder final
{
 public:
  // No copy/assignment.
  Encoder(const Encoder &) = delete;
  Encoder &operator=(const Encoder &) = delete;

  // Constructor will start the encoding process,
  // building the Huffman tree and creating the output stream.
  // Call getBitStreamWriter() to fetch the results.
  Encoder(const std::uint8_t *data, int dataSizeBytes,
          bool prependTreeToBitStream);

  // Find node can be used by a decoder to reconstruct
  // the original data from a bit stream of Huffman codes.
  const Node *findNodeForCode(Code code) const;

  // Get the bit stream generated from the data.
  // The stream will be prefixed with the Huffman tree codes
  // if prependTreeToBitStream was set in the constructor.
  const BitStreamWriter &getBitStreamWriter() const;
  BitStreamWriter &getBitStreamWriter();

  // Get the length in bits of the tree data prefixed
  // to the stream if prependTreeToBitStream was true.
  // Always rounded to a byte boundary.
  int getTreePrefixBits() const;

 private:
  // Priority queue comparator.
  // Places nodes with lower frequency first.
  struct NodeCmp
  {
    bool operator()(const Node *a, const Node *b) const
    {
      return a->frequency > b->frequency;
    }
  };

  // Queue stores pointers into the nodes[] array below.
  using PQueue = std::priority_queue<Node *, std::vector<Node *>, NodeCmp>;

  // Internal helpers:
  void buildHuffmanTree();
  void writeTreeBitStream();
  void writeDataBitStream(const std::uint8_t *data, int dataSizeBytes);
  void countFrequencies(const std::uint8_t *data, int dataSizeBytes);
  void recursiveAssignCodes(Node *node, const Node *parent, int bit);
  const Node *recursiveFindLeaf(const Node *node, Code code) const;
  Node *addInnerNode(int frequency, int child0, int child1);

 private:
  // Output bit stream (will allocate some heap memory).
  BitStreamWriter bitStream;

  Node *treeRoot;
  int treePrefixBits;

  // Fixed-size pool of nodes. We don't explicitly allocate memory in the
  // encoder.
  std::array<Node, MaxNodes> nodes;
};

// ========================================================
// Huffman decoder class:
// ========================================================

class Decoder final
{
 public:
  // No copy/assignment.
  Decoder(const Decoder &) = delete;
  Decoder &operator=(const Decoder &) = delete;

  // Start the decoder from a bit stream:
  explicit Decoder(const BitStreamWriter &encodedBitStream);
  Decoder(const std::uint8_t *encodedData, int encodedSizeBytes,
          int encodedSizeBits);

  // Runs the decoding loop, writing to the user buffer.
  // Returns the number of *bytes* decoded, which might differ
  // from dataSizeBytes if there is an error or size mismatch.
  int decode(std::uint8_t *data, int dataSizeBytes);

 private:
  // Internal helpers:
  void readPrefixData();
  int findMatchingCode(const Code code) const;

  // Helps us manipulate the external raw buffer.
  BitStreamReader bitStream;

  // Assume the output is only storing the leaf nodes.
  // We also don't need to store a full Node here, just
  // its code, since the value/symbol is implicit by the
  // position within the array.
  std::array<Code, MaxSymbols> codes;
  std::map<Code, int> codes_maps;
};

// ========================================================
// easyEncode() / easyDecode():
// ========================================================

// Quick Huffman data compression.
// Output compressed data is heap allocated with HUFFMAN_MALLOC()
// and should be later freed with HUFFMAN_MFREE().
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

}  // namespace huffman

// ================== End of header file ==================
#endif  // HUFFMAN_HPP
// ================== End of header file ==================

// ================================================================================================
//
//                                  Huffman Implementation
//
// ================================================================================================

#ifdef HUFFMAN_USING_DEFAULT_ERROR_HANDLER
#include <cstdio>  // For the default error handler
#endif             // HUFFMAN_USING_DEFAULT_ERROR_HANDLER

#include <cassert>
#include <cstring>
