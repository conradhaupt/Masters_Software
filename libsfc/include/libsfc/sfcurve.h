//
// Created by conrad on 2018/05/23.
//

#ifndef SFC_SFCURVE_H
#define SFC_SFCURVE_H

#include <algorithm>
#include <memory>
#include <type_traits>
#include "coordinates.h"
#include "iterator.h"
#include "pow.h"
#include "range.h"
#include "sfcdef.h"

namespace sfc {

template <sfc::size_t _NDim>
class sfcurve
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

  using index_type = sfc::size_t;
  using size_type = sfc::size_t;
  using difference_type = std::ptrdiff_t;
  using coord_type = index_type;
  using coords_type = sfc::coordinates<coord_type, _NDim>;
  using dist_type = index_type;
  using sfcurve_type = sfcurve<_NDim>;

  using iterator_type = sfc::iterator<_NDim, sfcurve>;

 private:
  const size_type _dimLength;
  const size_type _Nm;
  void __check_values()
  {
    static_assert(_NDim > 0, "number of dimensions must be a positive integer");
    if (_dimLength <= 1)
      throw std::invalid_argument(
          "dimension length must be a positive integer greater than 1");
    if (_Nm != sfc::pow(_dimLength, _NDim))
      throw std::runtime_error(
          "something went wrong. Number of elements is not correct");
  }

 protected:
  //  Bounds checking functions
  inline constexpr bool fallsWithinBounds(const coords_type &coords) const
  {
    return std::all_of(std::begin(coords), std::end(coords),
                       [this](index_type coord) { return coord < _dimLength; });
  }

  inline void throwIfFallsOutsideBounds(const coords_type &coords) const
  {
    if (!fallsWithinBounds(coords)) throw CoordsOutOfBoundsException();
  }

  inline constexpr bool fallsWithinBounds(const index_type &dist) const
  {
    return dist < _Nm;
  }

  inline void throwIfFallsOutsideBounds(const index_type &dist) const
  {
    if (!fallsWithinBounds(dist)) throw DistanceOutOfBoundsException();
  }

 public:
  sfcurve(size_type dimLength)
      : _dimLength{dimLength}, _Nm{sfc::pow(dimLength, _NDim)}
  {
    __check_values();
  }

  sfcurve(const sfcurve_type &sf) : _dimLength{sf._dimLength}, _Nm{sf._Nm} {}
  sfcurve(sfcurve_type &&sf) noexcept
      : _dimLength{std::move(sf._dimLength)}, _Nm{std::move(sf._Nm)}
  {}

  virtual ~sfcurve() {}

  constexpr size_type numberOfDimensions() const { return _NDim; }
  constexpr size_type totalElements() const { return _Nm; }
  constexpr size_type maxDistance() const { return _Nm - 1; }
  constexpr size_type dimensionLength() const { return _dimLength; }

  constexpr index_type coordsToIndex(const coords_type &coords) const
  {
    throwIfFallsOutsideBounds(coords);
    index_type index{0};
    index_type _prev_index{0};
    index_type _index_scaler{1};
    for (auto &&i : sfc::range(_NDim)) {
      _prev_index = index + coords.at(i) * _index_scaler;
      if (_prev_index < index)
        throw std::overflow_error(
            "index_type is not large enough to convert coordinates within "
            "bounds");
      _index_scaler *= _dimLength;
      index = _prev_index;
    }
    return index;
  }

  constexpr coords_type indexToCoords(const index_type &index) const
  {
    throwIfFallsOutsideBounds(index);
    coords_type coords{};
    index_type _index = index;
    for (auto i = 0; i < _NDim; i++) {
      coords[i] = _index % _dimLength;
      _index /= _dimLength;
    }
    return coords;
  }

  virtual size_type coordsToDist(const coords_type &coords) const = 0;
  virtual coords_type distToCoords(const dist_type &dist) const = 0;
  virtual std::unique_ptr<sfcurve_type> clone() const = 0;

  iterator_type begin() const { return iterator_type(clone(), 0); }
  constexpr iterator_type cbegin() const { return iterator_type(clone(), 0); }
  iterator_type end() const { return iterator_type(clone(), totalElements()); }
  constexpr iterator_type cend() const
  {
    return iterator_type(clone(), totalElements());
  }
};
}  // namespace sfc

#endif  // SFC_SFCURVE_H
