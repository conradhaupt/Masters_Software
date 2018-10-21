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
#include "hilbert_mapping.h"
#include "sfcdef.h"
#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim = 2>
class hilbert : public sfcurve<_NDim>
{
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
           coord_type ry) const
  {
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
  virtual size_type coordsToDist(const coords_type &coords) const override
  {
    auto distance =
        sfc::libhilbert::getHKeyFromIntCoord<_NDim>(dimensionLength(), coords);
    return distance;
  }

  // Implementation based on code from
  // https://people.sc.fsu.edu/~jburkardt/c_src/hilbert_curve/hilbert_curve.html
  virtual coords_type distToCoords(const dist_type &dist) const override
  {
    return libhilbert::getIntCoordFromHKey<_NDim, coords_type>(
        dimensionLength(), dist);
  }

  std::unique_ptr<sfcurve_type> clone() const override
  {
    return std::make_unique<hilbert_type>(*this);
  }
};
};  // namespace sfc

#endif
