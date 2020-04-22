/**
 * @file unsigned.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFC_UNSIGNED_H
#define SFC_UNSIGNED_H

#include <algorithm>
#include <cmath>
#include <type_traits>
namespace sfc {

/**
 * @brief Returns the absolute difference between two values if their types is
 * unsigned.
 *
 * @tparam _Tp the type for the values supplied
 * @param t1 the first value
 * @param t2 the second value
 * @return the absolute difference between the two values
 */
template <typename _Tp>
inline constexpr auto abs_diff(_Tp t1, _Tp t2)
    -> std::enable_if_t<std::is_unsigned_v<_Tp>, _Tp>
{
  return std::max(t1, t2) - std::min(t1, t2);
}

/**
 * @brief Returns the absolute difference between two values if their types is
 * signed.
 *
 * @tparam _Tp the type for the values supplied
 * @param t1 the first value
 * @param t2 the second value
 * @return the absolute difference between the two values
 */
template <typename _Tp>
inline constexpr auto abs_diff(_Tp t1, _Tp t2)
    -> std::enable_if_t<std::is_signed_v<_Tp>, _Tp>
{
  return std::abs(t2 - t1);
}
};  // namespace sfc

#endif