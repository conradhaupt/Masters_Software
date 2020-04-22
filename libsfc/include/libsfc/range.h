/**
 * @file range.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-05-31
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFC_RANGE_H
#define SFC_RANGE_H

#include <type_traits>
#include "sfcdef.h"

namespace sfc {
template <class T = ::sfc::size_t>
class range
{
 private:
  T last;
  T iter;

 public:
  explicit range(T begin, T end) : iter{begin}, last{end}
  {
    static_assert(std::is_integral_v<T>, "Integral required");
  }
  explicit range(T end) : range(T(), end) {}

  // Iterable functions
  const range &begin() const { return *this; }
  const range &end() const { return *this; }

  // Iterator functions
  bool operator!=(const range &) const { return iter < last; }
  void operator++() { ++iter; }
  T operator*() const { return iter; }
};

// Default type for sfc::range
template <>
class range<int>;

}  // namespace sfc

#endif  // SFC_RANGE_H
