/**
 * @file rectangle.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-23
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFC_RECTANGLE_H
#define SFC_RECTANGLE_H

#include <algorithm>
#include <functional>
#include "coordinates.h"
#include "sfcdef.h"
namespace sfc {
template <class T, sfc::size_t N>
class rectangle
{
 public:
  using coordinate_t = typename sfc::coordinates<T, N>;

 private:
  coordinate_t _minCoord;
  coordinate_t _maxCoord;

 public:
  rectangle(coordinate_t minCoord, coordinate_t maxCoord)
  {
    std::transform(std::begin(minCoord), std::end(minCoord),
                   std::begin(maxCoord), std::begin(_minCoord),
                   [](auto& a, auto& b) { return a > b ? b : a; });
    std::transform(std::begin(minCoord), std::end(minCoord),
                   std::begin(maxCoord), std::begin(_maxCoord),
                   [](auto& a, auto& b) { return a > b ? a : b; });
  }

  coordinate_t minCoord() const { return _minCoord; }
  coordinate_t maxCoord() const { return _maxCoord; }

  bool contains(const coordinate_t& coord)
  {
    return sfc::coords::greaterThanEqual(coord, _minCoord) &&
           sfc::coords::lessThanEqual(coord, _maxCoord);
  }

  void expandToInclude(const coordinate_t& coord)
  {
    std::transform(std::begin(_minCoord), std::end(_minCoord),
                   std::begin(coord), std::begin(_minCoord),
                   [](auto& a, auto& b) { return a > b ? b : a; });
    std::transform(std::begin(_maxCoord), std::end(_maxCoord),
                   std::begin(coord), std::begin(_maxCoord),
                   [](auto& a, auto& b) { return a > b ? a : b; });
  }

  constexpr T area() const
  {
    coordinate_t diff{};
    std::transform(std::begin(_minCoord), std::end(_minCoord),
                   std::begin(_maxCoord), std::begin(diff),
                   [](auto& min, auto& max) { return max - min + 1; });
    return std::accumulate(std::begin(diff), std::end(diff), T(1),
                           std::multiplies<T>());
  }
};
};     // namespace sfc
#endif /* RECTANGLE_H */
