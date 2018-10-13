/**
 * @brief exponent operations/code
 *
 * @file pow.h
 * @author Conrad Haupt
 * @date 2018-06-29
 */

#ifndef SFC_POW_H
#define SFC_POW_H

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace sfc {
template <typename Tbase, typename Texp>
constexpr auto pow(Tbase base, Texp exp)
    -> std::enable_if_t<std::is_integral_v<Tbase> && std::is_integral_v<Texp>,
                        Tbase>
{
  if (exp == 0) return 1;   // x^0 condition
  if (base == 0) return 0;  // 0^N condition

  Tbase result = 1;
  Tbase _result;
  while (exp > 0ULL) {
    _result = result * base;
    exp--;
    // If overflow occurs, throw an error
    if (_result < result)
      throw std::overflow_error("overflow occured for " + std::to_string(base) +
                                "^" + std::to_string(exp));
    result = _result;
  }
  return result;
}

template <typename T>
constexpr auto pow(T base, T exp) -> std::enable_if_t<!std::is_integral_v<T>, T>
{
  return std::pow(base, exp);
}

// Computes 2^exp
template <unsigned long long exp>
struct power_of_two
{
  enum
  {
    value = 1 << exp
  };
};

template <typename T, T exp>
inline constexpr T power_of_two_v = power_of_two<exp>::value;

template <typename T>
inline constexpr auto isMultipleOf(const T& val, const T& mult)
    -> std::enable_if_t<std::is_integral_v<T>, bool>
{
  return (val % mult) == 0;
}
};  // namespace sfc
#endif
