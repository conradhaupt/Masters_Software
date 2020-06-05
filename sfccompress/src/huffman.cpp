
// ================================================================================================
// -*- C++ -*-
// File: huffman.cpp (Modified from huffman.hpp by Conrad Haupt to be compilable
// and linkable) Author: Guilherme R. Lampert Created on: 15/02/16 Brief:
// Minimal implementation of Huffman encoding/decoding in C++11.
// ================================================================================================

#include "huffman.h"

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

// If you provide a custom malloc(), you must also provide a custom free().
// Note: We never check HUFFMAN_MALLOC's return for null. A custom
// implementation should just abort with a fatal error if the program runs out
// of memory.

namespace huffman {

// ========================================================
// class Code:
// ========================================================

Code::Code() { clear(); }

void Code::clear()
{
  bits = 0;
  length = 0;
}

void Code::appendBit(const int bit)
{
  if (length == MaxBits) {
    HUFFMAN_ERROR("Max code length exceeded!");
    return;
  }

  // Using one of the Stanford bit-hacks to set/clear the bit:
  // http://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
  const std::uint64_t mask = std::uint64_t(1) << length;
  bits = (bits & ~mask) | (-bit & mask);
  ++length;
}

void Code::appendCode(const Code other)
{
  for (int b = 0; b < other.getLength(); ++b) {
    appendBit(other.getBit(b));
  }
}

int Code::getBit(const int index) const
{
  const std::uint64_t mask = std::uint64_t(1) << index;
  return !!(bits & mask);
}

std::string Code::toBitString() const  // Useful for debugging.
{
  std::string bitString;
  for (int b = 0; b < getLength(); ++b) {
    bitString += (getBit(b) ? '1' : '0');
  }
  return bitString;
}

int Code::getLength() const { return length; }
void Code::setLength(const int lenInBits) { length = lenInBits; }

std::uint64_t Code::getAsU64() const { return bits; }
void Code::setAsU64(const std::uint64_t num) { bits = num; }

bool Code::operator==(const Code other) const
{
  return bits == other.bits && length == other.length;
}
bool Code::operator!=(const Code other) const { return !(*this == other); }

bool Code::operator<(const Code other) const
{
  if (bits < other.bits) {
    return true;
  } else if (bits == other.bits) {
    return length < other.length;
  } else {
    return false;
  }
}

// ========================================================
// class BitStreamReader:
// ========================================================

int BitStreamReader::getByteCount() const { return sizeInBytes; }
int BitStreamReader::getBitCount() const { return sizeInBits; }
const std::uint8_t *BitStreamReader::getBitStream() const { return stream; }

// Current Huffman code being read from the stream:
void BitStreamReader::clearCode() { currCode.clear(); }
Code BitStreamReader::getCode() const { return currCode; }
int BitStreamReader::getCodeLength() const { return currCode.getLength(); }

bool Node::isValid() const { return frequency != Nil; }
bool Node::isLeaf() const { return leftChild == Nil && rightChild == Nil; }

// ========================================================
// Huffman encoder class:
// ========================================================

}  // namespace huffman

// ================================================================================================
//
//                                  Huffman Implementation
//
// ================================================================================================

namespace huffman {

// ========================================================
// Local helpers:
// ========================================================

// Round up to the next power-of-two number, e.g. 37 => 64
static int nextPowerOfTwo(int num)
{
  --num;
  for (std::size_t i = 1; i < sizeof(num) * 8; i <<= 1) {
    num = num | num >> i;
  }
  return ++num;
}

// ========================================================

// Count the minimum number of bits required to
// represent the integer 'num', AKA its log2.
static int bitsForInteger(int num)
{
  int bits = 0;
  while (num > 0) {
    num = num >> 1;
    ++bits;
  }
  return bits;
}

// ========================================================

#ifdef HUFFMAN_USING_DEFAULT_ERROR_HANDLER

// Prints a fatal error to stderr and aborts the process.
// This is the default method used by HUFFMAN_ERROR(), but
// you can override the macro to use other error handling
// mechanisms, such as C++ exceptions.
void fatalError(const char *const message)
{
  std::fprintf(stderr, "Huffman encoder/decoder error: %s\n", message);
  std::abort();
}

#endif  // HUFFMAN_USING_DEFAULT_ERROR_HANDLER

// ========================================================
// class BitStreamWriter:
// ========================================================

BitStreamWriter::BitStreamWriter()
{
  // 8192 bits for a start (1024 bytes). It will resize if needed.
  // Default granularity is 2.
  internalInit();
  allocate(8192);
}

BitStreamWriter::BitStreamWriter(const int initialSizeInBits,
                                 const int growthGranularity)
{
  internalInit();
  setGranularity(growthGranularity);
  allocate(initialSizeInBits);
}

BitStreamWriter::~BitStreamWriter()
{
  if (stream != nullptr) {
    HUFFMAN_MFREE(stream);
  }
}

void BitStreamWriter::internalInit()
{
  stream = nullptr;
  bytesAllocated = 0;
  granularity = 2;
  currBytePos = 0;
  nextBitPos = 0;
  numBitsWritten = 0;
}

void BitStreamWriter::allocate(int bitsWanted)
{
  // Require at least a byte.
  if (bitsWanted <= 0) {
    bitsWanted = 8;
  }

  // Round upwards if needed:
  if ((bitsWanted % 8) != 0) {
    bitsWanted = nextPowerOfTwo(bitsWanted);
  }

  // We might already have the required count.
  const int sizeInBytes = bitsWanted / 8;
  if (sizeInBytes <= bytesAllocated) {
    return;
  }

  stream = allocBytes(sizeInBytes, stream, bytesAllocated);
  bytesAllocated = sizeInBytes;
}

void BitStreamWriter::appendBit(const int bit)
{
  const std::uint32_t mask = std::uint32_t(1) << nextBitPos;
  stream[currBytePos] = (stream[currBytePos] & ~mask) | (-bit & mask);
  ++numBitsWritten;

  if (++nextBitPos == 8) {
    nextBitPos = 0;
    if (++currBytePos == bytesAllocated) {
      allocate(bytesAllocated * granularity * 8);
    }
  }
}

void BitStreamWriter::appendBitsU64(const std::uint64_t num, const int bitCount)
{
  assert(bitCount <= 64);
  for (int b = 0; b < bitCount; ++b) {
    const std::uint64_t mask = std::uint64_t(1) << b;
    const int bit = !!(num & mask);
    appendBit(bit);
  }
}

void BitStreamWriter::appendCode(const Code code)
{
  for (int b = 0; b < code.getLength(); ++b) {
    appendBit(code.getBit(b));
  }
}

#ifndef HUFFMAN_NO_STD_STRING

void BitStreamWriter::appendBitString(const std::string &bitStr)
{
  for (std::size_t i = 0; i < bitStr.length(); ++i) {
    appendBit(bitStr[i] == '0' ? 0 : 1);
  }
}

std::string BitStreamWriter::toBitString() const
{
  std::string bitString;

  int usedBytes = numBitsWritten / 8;
  int leftovers = numBitsWritten % 8;
  if (leftovers != 0) {
    ++usedBytes;
  }
  assert(usedBytes <= bytesAllocated);

  for (int i = 0; i < usedBytes; ++i) {
    const int nBits =
        (leftovers == 0) ? 8 : (i == usedBytes - 1) ? leftovers : 8;
    for (int j = 0; j < nBits; ++j) {
      bitString += (stream[i] & (1 << j) ? '1' : '0');
    }
  }

  return bitString;
}

#endif  // HUFFMAN_NO_STD_STRING

std::uint8_t *BitStreamWriter::release()
{
  std::uint8_t *oldPtr = stream;
  internalInit();
  return oldPtr;
}

void BitStreamWriter::setGranularity(const int growthGranularity)
{
  granularity = (growthGranularity >= 2) ? growthGranularity : 2;
}

int BitStreamWriter::getByteCount() const
{
  int usedBytes = numBitsWritten / 8;
  int leftovers = numBitsWritten % 8;
  if (leftovers != 0) {
    ++usedBytes;
  }
  assert(usedBytes <= bytesAllocated);
  return usedBytes;
}

int BitStreamWriter::getBitCount() const { return numBitsWritten; }

const std::uint8_t *BitStreamWriter::getBitStream() const { return stream; }

std::uint8_t *BitStreamWriter::allocBytes(const int bytesWanted,
                                          std::uint8_t *oldPtr,
                                          const int oldSize)
{
  std::uint8_t *newMemory =
      static_cast<std::uint8_t *>(HUFFMAN_MALLOC(bytesWanted));
  std::memset(newMemory, 0, bytesWanted);

  if (oldPtr != nullptr) {
    std::memcpy(newMemory, oldPtr, oldSize);
    HUFFMAN_MFREE(oldPtr);
  }

  return newMemory;
}

// ========================================================
// class BitStreamReader:
// ========================================================

BitStreamReader::BitStreamReader(const BitStreamWriter &bitStreamWriter)
    : stream(bitStreamWriter.getBitStream()),
      sizeInBytes(bitStreamWriter.getByteCount()),
      sizeInBits(bitStreamWriter.getBitCount())
{
  reset();
}

BitStreamReader::BitStreamReader(const std::uint8_t *bitStream,
                                 const int byteCount, const int bitCount)
    : stream(bitStream), sizeInBytes(byteCount), sizeInBits(bitCount)
{
  reset();
}

bool BitStreamReader::readNextBit()
{
  if (numBitsRead > 45000000)
    // std::cout << numBitsRead << "\t" << sizeInBits << std::endl;
    if (numBitsRead >= sizeInBits) {
      return false;  // We are done.
    }

  const std::uint32_t mask = std::uint32_t(1) << nextBitPos;
  const int bit = !!(stream[currBytePos] & mask);
  auto prevNumBitsRead = numBitsRead;
  ++numBitsRead;
  if (numBitsRead < prevNumBitsRead) std::cout << "ERROR, ERROR!" << std::endl;

  if (++nextBitPos == 8) {
    nextBitPos = 0;
    ++currBytePos;
  }

  currCode.appendBit(bit);
  return true;
}

std::uint64_t BitStreamReader::readBitsU64(const int bitCount)
{
  assert(bitCount <= 64);

  // We can reuse the Code reading infrastructure for this.
  // This is arguably a little hackish, but gets the job done...
  currCode.clear();
  for (int b = 0; b < bitCount; ++b) {
    if (!readNextBit()) {
      HUFFMAN_ERROR("Failed to read bits from stream! Unexpected end.");
      break;
    }
  }
  return currCode.getAsU64();
}

void BitStreamReader::reset()
{
  currBytePos = 0;
  nextBitPos = 0;
  numBitsRead = 0;
  currCode.clear();
}

// ========================================================
// class Encoder:
// ========================================================

Encoder::Encoder(const std::uint8_t *data, const int dataSizeBytes,
                 const bool prependTreeToBitStream)
    : treeRoot(nullptr), treePrefixBits(0)
{
  countFrequencies(data, dataSizeBytes);
  buildHuffmanTree();

  if (prependTreeToBitStream) {
    writeTreeBitStream();
  }

  writeDataBitStream(data, dataSizeBytes);
}

void Encoder::buildHuffmanTree()
{
  PQueue pQueue;

  // Put each symbol node into a priority queue:
  for (int s = 0; s < MaxSymbols; ++s) {
    if (nodes[s].isValid()) {
      pQueue.push(&nodes[s]);
    }
  }

  // Build the tree using the following algorithm:
  //
  // While there is more than one node in the queue:
  //   - Remove the two nodes of highest priority (lowest frequency) from the
  //   queue;
  //   - Create a new internal node with these two nodes as children
  //     and with frequency equal to the sum of the two nodes' frequencies;
  //   - Add the new node to the queue;
  // Repeat;
  //
  // The remaining node is the root node and the tree is complete.
  //
  while (pQueue.size() > 1) {
    const auto child0 = pQueue.top();
    pQueue.pop();

    const auto child1 = pQueue.top();
    pQueue.pop();

    pQueue.push(addInnerNode(child0->frequency + child1->frequency,
                             child0->value, child1->value));
  }

  // Now we can assign the binary codes, starting from 0 at the root:
  assert(!pQueue.empty());
  treeRoot = pQueue.top();
  recursiveAssignCodes(treeRoot, nullptr, 0);
}

Node *Encoder::addInnerNode(const int frequency, const int leftChild,
                            const int rightChild)
{
  // Find a free slot:
  // First 256 nodes are reserved for the data symbols,
  // (leaf nodes) with the inner nodes following.
  for (int n = MaxSymbols; n < MaxNodes; ++n) {
    if (!nodes[n].isValid()) {
      nodes[n].frequency = frequency;
      nodes[n].leftChild = leftChild;
      nodes[n].rightChild = rightChild;
      nodes[n].value = n;
      return &nodes[n];
    }
  }

  HUFFMAN_ERROR("No more free node slots!");
  return &nodes[MaxNodes - 1];
}

void Encoder::recursiveAssignCodes(Node *node, const Node *parent,
                                   const int bit)
{
  // Inherit the parent code if not the root:
  if (parent != nullptr) {
    node->code.appendCode(parent->code);
  }

  // Add the bit for this node:
  node->code.appendBit(bit);

  // And continue with it's children:
  if (node->leftChild != Nil) {
    // Bit zero to the left.
    recursiveAssignCodes(&nodes[node->leftChild], node, 0);
  }
  if (node->rightChild != Nil) {
    // Bit one to the right.
    recursiveAssignCodes(&nodes[node->rightChild], node, 1);
  }
}

void Encoder::countFrequencies(const std::uint8_t *data, int dataSizeBytes)
{
  for (; dataSizeBytes > 0; --dataSizeBytes, ++data) {
    // We'll use the value of each byte as the symbol index, since our table has
    // 256+ entries.
    const int nodeIndex = *data;

    // First occurrence?
    if (!nodes[nodeIndex].isValid()) {
      nodes[nodeIndex].frequency = 1;
      nodes[nodeIndex].value = *data;
    } else  // Recurrent symbol:
    {
      nodes[nodeIndex].frequency++;
    }
  }
}

void Encoder::writeDataBitStream(const std::uint8_t *data, int dataSizeBytes)
{
  for (; dataSizeBytes > 0; --dataSizeBytes, ++data) {
    // We can index the nodes directly from each byte of data
    // since the first 256 slots are reserved for the symbols,
    // so Node::value is the same as its index in th array for
    // the first 256 leaf nodes.
    const int nodeIndex = *data;
    bitStream.appendCode(nodes[nodeIndex].code);
  }
}

void Encoder::writeTreeBitStream()
{
  assert(treeRoot != nullptr);

  //
  // The length used for the codes will be the shortest one possible.
  // We only write the leaf nodes, that's enough to reconstruct
  // the data later, so we will output 256 codes (MaxSymbols). The
  // index of each code is the implicit value/symbol of that node.
  //
  // The stream will be prefixed with two 16-bits words, the
  // number of codes (always 256/MaxSymbols for now) and the
  // width in bits of the code_length field that prefixes
  // every code. 256 code instances follow in the format:
  //
  // +-------------+---------------+
  // | code_length | code_bits ... |
  // +-------------+---------------+
  //   ^-- Fixed     ^-- Varying width
  //       width         (code_length bits)
  //

  // Find the longest code so we know the max number
  // of bits we will need to represent that value.
  int maxCodeLengthInBits = 0;
  for (int s = 0; s < MaxSymbols; ++s) {
    if (nodes[s].isValid() && nodes[s].code.getLength() > maxCodeLengthInBits) {
      maxCodeLengthInBits = nodes[s].code.getLength();
    }
  }

  // Code length is currently limited to uint64!
  if (maxCodeLengthInBits <= 0 || maxCodeLengthInBits > Code::MaxBits) {
    HUFFMAN_ERROR("Unexpected code length! Should be <= Code::MaxBits.");
    return;
  }

  // Write the counts:
  const int numberOfCodes = MaxSymbols;
  const int codeLengthWidth = bitsForInteger(maxCodeLengthInBits);
  bitStream.appendBitsU64(numberOfCodes, 16);
  bitStream.appendBitsU64(codeLengthWidth, 16);
  treePrefixBits = 32;  // 16 bits each.

  // Output the 256 nodes, in order:
  for (int s = 0; s < MaxSymbols; ++s) {
    // Write the length of the code using a fixed bit-width:
    const int codeLen = nodes[s].code.getLength();
    bitStream.appendBitsU64(codeLen, codeLengthWidth);

    // Write the code bits themselves, using a varying bit-width:
    const std::uint64_t codeNum = nodes[s].code.getAsU64();
    bitStream.appendBitsU64(codeNum, codeLen);
    // std::cout << "Code [" << s << "]: num=" << codeNum
    //           << ", codeLen=" << codeLen << std::endl;

    // Keep track of the number of bits written so far for later padding.
    treePrefixBits += (codeLengthWidth + codeLen);
  }

  // Pad to a full byte if needed:
  while ((treePrefixBits % 8) != 0) {
    bitStream.appendBit(0);
    ++treePrefixBits;
  }
}

const Node *Encoder::recursiveFindLeaf(const Node *node, const Code code) const
{
  const Node *result = nullptr;

  if (node->leftChild != Nil) {
    result = recursiveFindLeaf(&nodes[node->leftChild], code);
    if (result != nullptr) {
      return result;
    }
  }
  if (node->rightChild != Nil) {
    result = recursiveFindLeaf(&nodes[node->rightChild], code);
    if (result != nullptr) {
      return result;
    }
  }

  if (node->isLeaf() && node->code == code) {
    result = node;
  }
  return result;
}

const Node *Encoder::findNodeForCode(const Code code) const
{
  return recursiveFindLeaf(treeRoot, code);
}

const BitStreamWriter &Encoder::getBitStreamWriter() const { return bitStream; }

BitStreamWriter &Encoder::getBitStreamWriter() { return bitStream; }

int Encoder::getTreePrefixBits() const { return treePrefixBits; }

// ========================================================
// class Decoder:
// ========================================================

Decoder::Decoder(const BitStreamWriter &encodedBitStream)
    : bitStream(encodedBitStream)
{
  readPrefixData();
}

Decoder::Decoder(const std::uint8_t *encodedData, const int encodedSizeBytes,
                 const int encodedSizeBits)
    : bitStream(encodedData, encodedSizeBytes, encodedSizeBits)
{
  readPrefixData();
}

void Decoder::readPrefixData()
{
  // First two 16-bits words in the stream are
  // the number of codes, which must be 256, and
  // the width in bits of each code_length field.
  const std::uint64_t numberOfCodes = bitStream.readBitsU64(16);
  const std::uint64_t codeLengthWidth = bitStream.readBitsU64(16);
  int treePrefixBits = 32;  // The 16 bits read above.

  if (numberOfCodes != MaxSymbols) {
    HUFFMAN_ERROR("Unexpected code count in input bit stream! Should be 256.");
    return;
  }

  // 256/MaxSymbols codes follow:
  for (std::uint64_t c = 0; c < numberOfCodes; ++c) {
    //
    // Read the code_length field, fixed bit-width:
    //
    bitStream.clearCode();
    for (std::uint64_t l = 0; l < codeLengthWidth; ++l) {
      if (!bitStream.readNextBit()) {
        HUFFMAN_ERROR(
            "Failed to read code length from stream! Unexpected end.");
        return;
      }
    }
    treePrefixBits += codeLengthWidth;
    const std::uint64_t codeBitsWidth = bitStream.getCode().getAsU64();
    assert(codeBitsWidth <= Code::MaxBits);

    //
    // Now read the code bits using the just acquired length:
    //
    bitStream.clearCode();
    for (std::uint64_t l = 0; l < codeBitsWidth; ++l) {
      if (!bitStream.readNextBit()) {
        HUFFMAN_ERROR("Failed to read code bits from stream! Unexpected end.");
        return;
      }
    }
    treePrefixBits += codeBitsWidth;

    // Store the new code:
    codes[c] = bitStream.getCode();
  }

  // There might be some padding left that must be skipped:
  bitStream.clearCode();
  while ((treePrefixBits % 8) != 0) {
    bitStream.readNextBit();
    ++treePrefixBits;
  }
  bitStream.clearCode();

  // Populate codes_maps
  // codes_maps does not replace codes and instead is only used in
  // findMatchingCodes.
  for (auto i = 0; i < numberOfCodes; i++) {
    auto it = codes_maps.find(codes[i]);
    if (it == std::end(codes_maps)) {
      codes_maps.insert({codes[i], i});
    }
  }
}

int Decoder::findMatchingCode(const Code code) const
{
  auto it_map = codes_maps.find(code);
  if (it_map == std::end(codes_maps))
    return Nil;
  else
    return it_map->second;
}

int Decoder::decode(std::uint8_t *data, const int dataSizeBytes)
{
  assert(data != nullptr);
  assert(dataSizeBytes != 0);

  int bytesDecoded = 0;
  while (bitStream.readNextBit() && bytesDecoded < dataSizeBytes) {
    // See if the current code is valid:
    if (bytesDecoded == 8388607) {
      auto i = 0;
    }
    const int codeIndex = findMatchingCode(bitStream.getCode());
    if (codeIndex == Nil) {
      continue;
    }

    if (bytesDecoded == dataSizeBytes) {
      // HUFFMAN_ERROR("Decoder output buffer too small!");
      std::cout << "Decoder output buffer too small: " << bytesDecoded << "\t"
                << bitStream.getBitCount() << "\t" << bitStream.getByteCount()
                << std::endl;
      // break;
    }

    if (bytesDecoded < dataSizeBytes)
      *data++ = static_cast<std::uint8_t>(codeIndex);
    // else
    //   std::cout << bytesDecoded << std::endl;
    ++bytesDecoded;

    bitStream.clearCode();
  }

  return bytesDecoded;
}

// ========================================================
// easyEncode() implementation:
// ========================================================

void easyEncode(const std::uint8_t *uncompressed,
                const int uncompressedSizeBytes, std::uint8_t **compressed,
                int *compressedSizeBytes, int *compressedSizeBits)
{
  if (uncompressed == nullptr || compressed == nullptr) {
    HUFFMAN_ERROR("huffman::easyEncode(): Null data pointer(s)!");
    return;
  }

  if (uncompressedSizeBytes <= 0 || compressedSizeBytes == nullptr ||
      compressedSizeBits == nullptr) {
    HUFFMAN_ERROR("huffman::easyEncode(): Bad in/out sizes!");
    return;
  }

  Encoder encoder(uncompressed, uncompressedSizeBytes,
                  /* prependTreeToBitStream = */ true);
  auto &bitStream = encoder.getBitStreamWriter();

  // Pass ownership of the compressed data buffer to the user pointer:
  *compressedSizeBytes = bitStream.getByteCount();
  *compressedSizeBits = bitStream.getBitCount();
  *compressed = bitStream.release();
}

// ========================================================
// easyDecode() implementation:
// ========================================================

int easyDecode(const std::uint8_t *compressed, const int compressedSizeBytes,
               const int compressedSizeBits, std::uint8_t *uncompressed,
               const int uncompressedSizeBytes)
{
  if (compressed == nullptr || uncompressed == nullptr) {
    HUFFMAN_ERROR("huffman::easyDecode(): Null data pointer(s)!");
    return 0;
  }

  if (compressedSizeBytes <= 0 || compressedSizeBits <= 0 ||
      uncompressedSizeBytes <= 0) {
    HUFFMAN_ERROR("huffman::easyDecode(): Bad in/out sizes!");
    return 0;
  }

  Decoder decoder(compressed, compressedSizeBytes, compressedSizeBits);
  return decoder.decode(uncompressed, uncompressedSizeBytes);
}

}  // namespace huffman

// ================ End of implementation =================
// ================ End of implementation =================