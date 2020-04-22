/**
 * @file bitstream.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "bitstream.h"
namespace sfcc {

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
    throw std::runtime_error("Max huffman code length exceeded!");
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
    delete[] stream;
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
    bitsWanted = ::sfcc::utils::nextPowerOfTwo(bitsWanted);
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
  std::uint8_t *newMemory = new std::uint8_t[bytesWanted];

  if (oldPtr != nullptr) {
    std::copy(oldPtr, oldPtr + oldSize, newMemory);
    delete[] oldPtr;
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
      throw std::runtime_error(
          "Failed to read bits from stream! Unexpected end.");
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

}  // namespace sfcc
