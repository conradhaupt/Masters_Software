/**
 * @brief implementation of morton order/z-order curve
 *
 * @file morton.h
 * @author Conrad Haupt
 * @date 2018-07-03
 */

#ifndef SFC_MORTON_H
#define SFC_MORTON_H

#include <limits>
#include <type_traits>
#include "base2.h"
#include "interleave.h"
#include "sfcdef.h"
#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim>
class morton : public sfcurve<_NDim>
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
  using morton_type = morton<_NDim>;

  using sfcurve_type::dimensionLength;
  using sfcurve_type::numberOfDimensions;
  using sfcurve_type::totalElements;

 private:
  void __check_values()
  {
    // Check if the dimension length allows for coord_type to be stored in
    // dist_type
    if (sfc::max_interleave_dim_length<_NDim, dist_type, index_type>() <
        dimensionLength())
      throw std::invalid_argument(
          "dimLength is too large, cannot store all indices in current "
          "datatypes");
    // This isn't necessary BUT it allows for no distances to not exist in the
    // coordinate space
    if (!sfc::isPowerOfTwo<size_type>(dimensionLength()))
      throw std::invalid_argument(
          "dimLength must be a power of two, this restriction will be "
          "deprecated in the future");
  }

 public:
  morton(size_type dimLength) : sfcurve_type{dimLength} {}
  morton(const morton_type &mn) : sfcurve_type(mn) {}
  morton(morton_type &&mn) noexcept : sfcurve_type(mn) {}

  virtual ~morton() {}

  virtual dist_type coordsToDist(const coords_type &coords) const override
  {
    return sfc::interleaveBits<_NDim, dist_type, coords_type, coord_type>(
        coords);
  }
  virtual coords_type distToCoords(const dist_type &dist) const override
  {
    return sfc::uinterleaveBits<_NDim, dist_type, coords_type, coord_type>(
        dist);
  }

  std::unique_ptr<sfcurve_type> clone() const override
  {
    return std::make_unique<morton_type>(*this);
  }
};
};  // namespace sfc

#endif
