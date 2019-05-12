/**
 * @file raster.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-05-10
 *
 * @copyright Copyright (c) 2019
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 *
 */
#ifndef SFC_RASTER_H
#define SFC_RASTER_H

#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim>
class raster : public sfcurve<_NDim>
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
  using raster_type = raster<_NDim>;

  using sfcurve_type::dimensionLength;
  using sfcurve_type::numberOfDimensions;
  using sfcurve_type::totalElements;

 public:
  raster(size_type dimLength) : sfcurve_type{dimLength} {}
  raster(const raster_type &rstr) : sfcurve_type(rstr) {}
  raster(raster_type &&rstr) noexcept : sfcurve_type(rstr) {}

  virtual ~raster() {}

  virtual dist_type coordsToDist(const coords_type &coords) const override
  {
    return sfcurve_type::coordsToIndex(coords);
  }
  virtual coords_type distToCoords(const dist_type &dist) const override
  {
    return sfcurve_type::indexToCoords(dist);
  }

  std::unique_ptr<sfcurve_type> clone() const override
  {
    return std::make_unique<raster_type>(*this);
  }
};
};  // namespace sfc

#endif /* RASTER_H */
