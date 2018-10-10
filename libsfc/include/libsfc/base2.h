/**
 * @brief base-2/binary operations/code
 *
 * @file base2.h
 * @author Conrad Haupt
 * @date 2018-06-29
 */

#ifndef SFC_BASE2_H
#define SFC_BASE2_H
#include <stdexcept>
#include <type_traits>

namespace sfc {
/**
 * @brief returns whether the given value is a power of two. 0 results in false.
 *
 * @tparam T the value type, must be integral
 * @param val the value to identify as a power-of-two or not
 * @return bool
 */
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, bool> isPowerOfTwo(
    const T& val) {
  if (val == 0) {
    return false;
  }
  return (val & (val - 1)) == 0;
}

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, T> ilog2(T val) {
  // throw error if not a power of two
  if (!isPowerOfTwo(val))
    throw std::invalid_argument("val must be of the form 2^N where N>=0");
  T N = 0;
  while (val > 1) {
    val /= 2;
    N++;
  }
  return N;
}
};  // namespace sfc

#endif
