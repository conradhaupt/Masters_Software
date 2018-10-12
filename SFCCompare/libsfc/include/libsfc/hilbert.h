/**
 * @brief implementation of morton order/z-order curve
 *
 * @file hilbert.h
 * @author Conrad Haupt
 * @date 2018-08-09
 */

#ifndef SFC_HILBERT_H
#define SFC_HILBERT_H

#include <limits>
#include <type_traits>
#include "base2.h"
#include "sfcdef.h"
#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim = 2>
class hilbert : public sfcurve<_NDim> {
 public:
  // using value_type = _Tp;
  // using pointer = value_type *;
  // using const_pointer = const value_type *;
  // using reference = value_type &;
  // using const_reference = const value_type &;
  // using iterator = value_type *;
  // using const_iterator = const value_type *;
  // using reverse_iterator = std::reverse_iterator<iterator>;
  // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using sfcurve_type = sfcurve<_NDim>;
  using index_type = typename sfcurve_type::index_type;
  using size_type = typename sfcurve_type::size_type;
  using difference_type = typename sfcurve_type::difference_type;
  using coord_type = typename sfcurve_type::coord_type;
  using coords_type = typename sfcurve_type::coords_type;
  using dist_type = typename sfcurve_type::dist_type;
  using hilbert_type = hilbert<_NDim>;

 private:
  // void __check_values() {
  //   // Check if the dimension length allows for coord_type to be stored in
  //   // dist_type
  //   if (sfc::max_interleave_dim_length<_NDim, dist_type, index_type>() <
  //       dimensionLength())
  //     throw std::invalid_argument(
  //         "dimLength is too large, cannot store all indices in current "
  //         "datatypes");
  //   // This isn't necessary BUT it allows for no distances to not exist in
  //   the
  //   // coordinate space
  //   if (!sfc::isPowerOfTwo<size_type>(dimensionLength()))
  //     throw std::invalid_argument(
  //         "dimLength must be a power of two, this restriction will be "
  //         "deprecated in the future");
  // }

  // Implementation based on code from
  // https://people.sc.fsu.edu/~jburkardt/c_src/hilbert_curve/hilbert_curve.html
  void rot(size_type n, coord_type &x, coord_type &y, coord_type rx,
           coord_type ry) const {
    size_type t;

    if (ry == 0) {
      if (rx == 1) {
        x = n - 1 - x;
        y = n - 1 - y;
      }
      std::swap(x, y);
    }
  }

 public:
  hilbert(size_type dimLength) : sfcurve_type{dimLength} {}
  hilbert(const hilbert_type &mn) : sfcurve_type(mn) {}
  hilbert(hilbert_type &&mn) noexcept : sfcurve_type(mn) {}

  virtual ~hilbert() {}

  using sfcurve_type::dimensionLength;
  using sfcurve_type::numberOfDimensions;
  using sfcurve_type::totalElements;

  // Implementation based on code from
  // https://people.sc.fsu.edu/~jburkardt/c_src/hilbert_curve/hilbert_curve.html
  virtual size_type coordsToDist(const coords_type &coords) const override {
    auto x = coords.at(0);
    auto y = coords.at(1);
    size_type d = 0;
    size_type n;
    size_type rx;
    size_type ry;
    size_type s;

    auto n_2 = sfc::pow(2, dimensionLength() - 1);
    for (s = n_2; s > 0; s = s / 2) {
      rx = (x & s) > 0;
      ry = (y & s) > 0;
      d = d + s * s * ((3 * rx) ^ ry);
      rot(s, x, y, rx, ry);
    }
    return d;
  }

  // Implementation based on code from
  // https://people.sc.fsu.edu/~jburkardt/c_src/hilbert_curve/hilbert_curve.html
  virtual coords_type distToCoords(const dist_type &dist) const override {
    coord_type x;
    coord_type y;
    coord_type rx;
    coord_type ry;
    size_type s;
    dist_type t = dist;

    auto n = sfc::pow(2, dimensionLength());

    x = 0;
    y = 0;
    for (s = 1; s < n; s = s * 2) {
      rx = 1 & (t / 2);
      ry = 1 & (t ^ rx);
      rot(s, x, y, rx, ry);
      x = x + s * rx;
      y = y + s * ry;
      t = t / 4;
    }
    return {x, y};
  }
};
};  // namespace sfc

#endif
