/**
 * @file utils.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-09-13
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef LIBSFCC_UTILS_H
#define LIBSFCC_UTILS_H

#include <cstddef>
namespace sfcc::utils {

// Round up to the next power-of-two number, e.g. 37 => 64
template <typename T>
constexpr T nextPowerOfTwo(T num)
{
  --num;
  for (std::size_t i = 1; i < sizeof(T) * 8; i <<= 1) {
    num = num | num >> i;
  }
  return ++num;
}

}  // namespace sfcc::utils

#endif /* UTILS_H */
