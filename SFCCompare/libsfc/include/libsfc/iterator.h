/**
 * @file iterator.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief Contains implementations of iterators that traverse space-filling
 * curves.
 * @version 0.1
 * @date 2018-10-12
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef SFC_ITERATOR_H
#define SFC_ITERATOR_H

#include <iterator>
#include <memory>
#include <utility>
#include "sfcdef.h"

namespace sfc {

// Forward declaration of sfcurve
template <sfc::size_t>
class sfcurve;

/**
 * @brief A point in a curve space that contains values defining its position in
 * the space as well as the distance along the given curve.
 *
 * @tparam _NDim the number of dimensions in the curve space
 * @tparam _TpCoord the type to be used for each dimension coordinate
 * @tparam _TpDist the type to be used for the distance along the given curve
 */
template <class T, sfc::size_t N>
using coordinates_ = std::array<T, N>;

template <sfc::size_t _NDim, typename _TpCoord, typename _TpDist>
struct point
{
  _TpDist distance;
  sfc::coordinates_<_TpCoord, _NDim> coords;

  bool operator==(const point<_NDim, _TpCoord, _TpDist>& p)
  {
    return (distance == p.distance) && (coords == p.coords);
  }
};

template <sfc::size_t _NDim,
          template <sfc::size_t> typename _TpCurve = sfc::sfcurve>
class iterator
{
 public:
  // sfc::sfcurve types
  using coord_type = typename _TpCurve<_NDim>::coord_type;
  using coords_type = typename _TpCurve<_NDim>::coords_type;
  using index_type = typename _TpCurve<_NDim>::index_type;
  using size_type = typename _TpCurve<_NDim>::size_type;
  using dist_type = typename _TpCurve<_NDim>::dist_type;

  // sfc::iterator types relating to sfc::sfcurve
  using point_type = sfc::point<_NDim, coord_type, dist_type>;

  // sfc::iterator types necessary for c++17 iterators
  using iterator_type = iterator<_NDim>;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = point_type;
  using pointer = value_type;
  using reference = value_type;

 private:
  std::unique_ptr<_TpCurve<_NDim>> _curve;
  dist_type _dist;
  coords_type _coords;

  inline bool distOutOfBounds() const
  {
    return (_dist < 0) || (_dist > _curve->maxDistance());
  }

  inline void throwIfDistOutOfBounds() const
  {
    if (distOutOfBounds()) throw OutOfBoundsException();
  }

  inline void updateCoordsFromDistIfInBounds()
  {
    if (!distOutOfBounds()) _coords = _curve->distToCoords(_dist);
  }

 public:
  /**
   * @brief Construct a new iterator object from an instance of a sfcurve or any
   * derived class.
   *
   * @tparam TpCurve the curve type
   * @param curve the curve instance
   */
  template <class TpCurve>
  iterator(const TpCurve& curve, const dist_type& dist)
      : _curve(std::make_unique<TpCurve>(curve)), _dist{dist}
  {
    updateCoordsFromDistIfInBounds();
    static_assert(std::is_base_of_v<sfc::sfcurve<_NDim>, TpCurve>);
  }

  template <class TpCurve>
  iterator(const TpCurve& curve) : iterator(curve, dist_type{})
  {
    static_assert(std::is_base_of_v<sfc::sfcurve<_NDim>, TpCurve>);
  }

  iterator(std::unique_ptr<sfc::sfcurve<_NDim>>&& curve, const dist_type& dist)
      : _curve{std::move(curve)}, _dist{dist}
  {
    updateCoordsFromDistIfInBounds();
  }

  // Copy constructor to achieve CopyConstructible requirements
  iterator(const iterator_type& it)
      : iterator_type(it._curve->clone(), it._dist)
  {}

  // Copy assignment operator to achieve CopyAssignable requirements
  iterator_type& operator=(const iterator_type& it)
  {
    _curve = it._curve->clone();
    _dist = it._dist;
    _coords = it._coords;
    return *this;
  }

  virtual ~iterator() {}

  point_type operator*() const
  {
    throwIfDistOutOfBounds();
    return point_type{_dist, _coords};
  }

  dist_type distance() const { return _dist; }
  coords_type coordinates() const
  {
    throwIfDistOutOfBounds();
    return _coords;
  }

  bool operator==(const iterator_type& it) const
  {
    if (_dist != it._dist) return false;
    if (!distOutOfBounds()) {
      return it._coords == it._coords;
    } else {
      return true;
    }
  }

  bool operator!=(const iterator_type& it) const
  {
    return !(this->operator==(it));
  }

  // Explicit swap function to eliminate any need for a friend function
  void swap(iterator_type&& it)
  {
    std::swap(_dist, it._dist);
    std::swap(_coords, it._coords);
    std::swap(_curve, it._curve);
  }

  bool outofbounds() const { return distOutOfBounds(); }

  // OPERATORS
  iterator_type& operator++()
  {
    _dist++;
    updateCoordsFromDistIfInBounds();
    return *this;
  }
  iterator_type operator++(int)
  {
    iterator_type tmp = *this;
    operator++();
    return tmp;
  }
  iterator_type& operator--()
  {
    _dist--;
    updateCoordsFromDistIfInBounds();
    return *this;
  }

  iterator_type operator--(int)
  {
    iterator_type tmp = *this;
    operator--();
    return tmp;
  }

  iterator_type& operator+=(const difference_type& n)
  {
    _dist += n;
    updateCoordsFromDistIfInBounds();
    return *this;
  }

  iterator_type& operator-=(const difference_type& n)
  {
    _dist -= n;
    updateCoordsFromDistIfInBounds();
    return *this;
  }

  iterator_type operator+(const difference_type& n)
  {
    auto tmp = *this;
    tmp._dist += n;
    tmp.updateCoordsFromDistIfInBounds();
    return tmp;
  }

  iterator_type operator-(const difference_type& n)
  {
    auto tmp = *this;
    tmp._dist -= n;
    tmp.updateCoordsFromDistIfInBounds();
    return tmp;
  }

  point_type operator[](const difference_type& n)
  {
    auto it = this + n;
    it.throwIfDistOutOfBounds();
    return *it;
  }

  point_type operator->() { return point_type{_dist, _coords}; }

  bool operator<(const iterator_type& it) { return _dist < it._dist; }
  bool operator>(const iterator_type& it) { return _dist > it._dist; }
  bool operator<=(const iterator_type& it) { return _dist <= it._dist; }
  bool operator>=(const iterator_type& it) { return _dist >= it._dist; }
};

template <sfc::size_t _NDim>
bool operator==(const iterator<_NDim>& it1, const iterator<_NDim>& it2)
{
  return it1.operator==(it2);
}

template <sfc::size_t _NDim>
bool operator!=(const iterator<_NDim>& it1, const iterator<_NDim>& it2)
{
  return it1.operator!=(it2);
}
};  // namespace sfc

template <sfc::size_t _NDim,
          template <sfc::size_t> typename _TpCurve = sfc::sfcurve>
auto operator-(const sfc::iterator<_NDim, _TpCurve>& it1,
               const sfc::iterator<_NDim, _TpCurve>& it2) ->
    typename sfc::iterator<_NDim, _TpCurve>::difference_type
{
  return (*it1).distance - (*it2).distance;
}
#endif