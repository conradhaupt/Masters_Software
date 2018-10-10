//
// Created by conrad on 2018/05/31.
//

#ifndef SFC_RANGE_H
#define SFC_RANGE_H

#include <type_traits>

namespace sfc {
template <class T>
class range {
 private:
  T last;
  T iter;

 public:
  explicit range(T end) : last(end), iter(T()) {
    static_assert(std::is_integral_v<T>, "Integral required");
  }

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
