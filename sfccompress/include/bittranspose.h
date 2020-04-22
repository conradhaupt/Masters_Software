/**
 * @file bittranspose.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-11-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFCC_BITTRANPOSE_H
#define SFCC_BITTRANPOSE_H
#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <iostream>
namespace sfcc {
template <typename T>
void bittranspose(T* input, T** output)
{
  const auto blocksize = sizeof(T) * CHAR_BIT;
  // For each bit position
  for (auto i = 0; i < blocksize; i++) {
    // For each value
    auto& output_val = *output[i];
    for (auto v = 0; v < blocksize; v++) {
      // Select the bit from input[v] and move to the appropriate position using
      // the offset from input[0]
      if (v >= i)
        output_val |= (input[v] & (T(1) << i)) << (v - i);
      else
        output_val |= (input[v] & (T(1) << i)) >> (i - v);
    }
  }
}

template <typename T>
void reverse_bittranspose(T** input, T* output)
{
  const auto blocksize = sizeof(T) * CHAR_BIT;
  // For each bit position
  for (auto i = 0; i < blocksize; i++) {
    // For each value
    auto& output_val = output[i];
    for (auto v = 0; v < blocksize; v++) {
      // Select the bit from input[v] and move to the appropriate position using
      // the offset from input[0]
      if (v >= i)
        output_val |= (*input[v] & (T(1) << i)) << (v - i);
      else
        output_val |= (*input[v] & (T(1) << i)) >> (i - v);
    }
  }
}

class BitTransposer
{
 public:
  BitTransposer() {}

  template <typename T>
  void transpose_t(T* data, unsigned long long length)
  {
    const auto nbits = sizeof(T) * CHAR_BIT;
    // Number of T in block
    const auto blocklength = length / (sizeof(T) * CHAR_BIT);
    T** transpose_output = new T*[nbits];
    T* output_data = new T[length]();
    for (auto i = 0; i < nbits; i++)
      transpose_output[i] = output_data + (blocklength * i);

    auto current_read = data;
    for (auto i = 0; i < blocklength; i++) {
      sfcc::bittranspose(current_read, transpose_output);
      current_read += nbits;
      std::for_each(transpose_output, transpose_output + nbits,
                    [](auto& out) { out++; });
    }
    std::copy(output_data, output_data + length, data);
    delete[] output_data;
    delete[] transpose_output;
  }
  template <typename T>
  void reverse_transpose_t(T* data, unsigned long long length)
  {
    const auto nbits = sizeof(T) * CHAR_BIT;
    // Number of T in block
    const auto blocklength = length / (sizeof(T) * CHAR_BIT);
    T** transpose_input = new T*[nbits];
    T* output_data = new T[length]();
    for (auto i = 0; i < nbits; i++)
      transpose_input[i] = data + (blocklength * i);

    auto current_write = output_data;
    for (auto i = 0; i < blocklength; i++) {
      sfcc::reverse_bittranspose(transpose_input, current_write);
      current_write += nbits;
      std::for_each(transpose_input, transpose_input + nbits,
                    [](auto& out) { out++; });
    }

    std::copy(output_data, output_data + length, data);
    delete[] output_data;
    delete[] transpose_input;
  }

  void transpose(std::uint8_t* data, unsigned long long length,
                 std::uint8_t dtype_nbytes)
  {
    if (length % dtype_nbytes != 0)
      throw std::logic_error("Length must be divisible by dtype_nbytes");
    switch (dtype_nbytes) {
      case 2:
        transpose_t<std::uint16_t>((std::uint16_t*)data, length / dtype_nbytes);
        return;
      case 4:
        transpose_t<std::uint32_t>((std::uint32_t*)data, length / dtype_nbytes);
        return;
      default:
        throw std::logic_error("dtype_nbytes value is not supported");
    }
  }

  void reverse_transpose(std::uint8_t* data, unsigned long long length,
                         std::uint8_t dtype_nbytes)
  {
    if (length % dtype_nbytes != 0)
      throw std::logic_error("Length must be divisible by dtype_nbytes");
    switch (dtype_nbytes) {
      case 2:
        reverse_transpose_t<std::uint16_t>((std::uint16_t*)data,
                                           length / dtype_nbytes);
        return;
      case 4:
        reverse_transpose_t<std::uint32_t>((std::uint32_t*)data,
                                           length / dtype_nbytes);
        return;
      default:
        throw std::logic_error("dtype_nbytes value is not supported");
    }
  }
};
}  // namespace sfcc

#endif