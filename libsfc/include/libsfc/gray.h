/**
 * @file gray.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-05
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFC_GRAY_H
#define SFC_GRAY_H

#include <initializer_list>
#include <limits>
#include <type_traits>
#include "base2.h"
#include "morton.h"
#include "sfcdef.h"

namespace sfc {

namespace internal {

template <typename _TpDist>
inline constexpr _TpDist __gray_decode_32(_TpDist dist)
{
  dist ^= (dist >> 16);
  dist ^= (dist >> 8);
  dist ^= (dist >> 4);
  dist ^= (dist >> 2);
  dist ^= (dist >> 1);
  return dist;
}

template <typename _TpDist>
inline constexpr _TpDist __gray_decode_64(_TpDist dist)
{
  dist ^= (dist >> 32);
  dist ^= (dist >> 16);
  dist ^= (dist >> 8);
  dist ^= (dist >> 4);
  dist ^= (dist >> 2);
  dist ^= (dist >> 1);
  return dist;
}

template <typename _TpDist, typename enable = std::enable_if<
                                (::sfc::sizeof_bits<_TpDist> == 32) ||
                                (::sfc::sizeof_bits<_TpDist> == 64)>>
constexpr _TpDist __gray_decode(_TpDist dist)
{
  if (::sfc::sizeof_bits<_TpDist> == 32)
    return __gray_decode_32(dist);
  else
    return __gray_decode_64(dist);
}
};  // namespace internal

template <sfc::size_t _NDim>
class gray : public morton<_NDim>
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
  using morton_type = morton<_NDim>;
  using index_type = typename sfcurve_type::index_type;
  using size_type = typename sfcurve_type::size_type;
  using difference_type = typename sfcurve_type::difference_type;
  using coord_type = typename sfcurve_type::coord_type;
  using coords_type = typename sfcurve_type::coords_type;
  using dist_type = typename sfcurve_type::dist_type;
  using gray_type = gray<_NDim>;

 private:
  using morton_type::dimensionLength;
  using morton_type::numberOfDimensions;
  using morton_type::totalElements;

  dist_type distToGrayDist(const dist_type &dist) const
  {
    return dist ^ (dist >> 1);
  }
  dist_type grayDistToDist(const dist_type &gray_dist) const
  {
    return ::sfc::internal::__gray_decode(gray_dist);
  }

 public:
  gray(size_type dimLength) : morton_type{dimLength} {}
  gray(const gray_type &mn) : morton_type(mn) {}
  gray(gray_type &&mn) noexcept : morton_type(mn) {}

  virtual ~gray() {}

  virtual dist_type coordsToDist(const coords_type &coords) const override
  {
    auto dist = morton_type::coordsToDist(coords);
    return distToGrayDist(dist);
  }
  virtual coords_type distToCoords(const dist_type &dist) const override
  {
    auto grayDist = grayDistToDist(dist);
    return morton_type::distToCoords(grayDist);
  }

  std::unique_ptr<sfcurve_type> clone() const override
  {
    return std::make_unique<gray_type>(*this);
  }
};
};  // namespace sfc

#endif
