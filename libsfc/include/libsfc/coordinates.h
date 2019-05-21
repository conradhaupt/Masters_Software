/**
 * @file coordinates.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-10-16
 *
 * @copyright Copyright (c) 2018
 *
 */
#ifndef SFC_COORDINATES_H
#define SFC_COORDINATES_H

#include <algorithm>
#include <array>
#include <numeric>
#include "range.h"
#include "sfcdef.h"
#include "unsigned.h"

namespace sfc {

/**
 * @brief Enum containing the definitions of various norms.
 *
 */
enum class NORM
{
  FIRST,    // Manhatten distance, i.e. sum of components
  SECOND,   // Euclidean distance, i.e. sqrt of the sum of squares
  INFINITE  // Infinite norm, i.e. max component
};

template <class T, sfc::size_t N>
using coordinates = std::array<T, N>;

namespace coords {
template <class T, sfc::size_t N>
T ManhattenDistanceTo(const coordinates<T, N>& coords1,
                      const coordinates<T, N>& coords2)
{
  // Get the absolute difference vector
  coordinates<T, N> res_diff;
  std::transform(std::begin(coords1), std::end(coords1), std::begin(coords2),
                 std::begin(res_diff), [](const auto& t1, const auto& t2) {
                   return sfc::abs_diff(t1, t2);
                 });
  // Get the sum of the difference components
  return std::accumulate(std::begin(res_diff), std::end(res_diff), T{});
}

template <class T, sfc::size_t N>
double EuclideanDistanceTo(const coordinates<T, N>& coords1,
                           const coordinates<T, N>& coords2)
{
  // Get the absolute difference vector
  coordinates<T, N> res_diff;
  std::transform(std::begin(coords1), std::end(coords1), std::begin(coords2),
                 std::begin(res_diff), [](const auto& t1, const auto& t2) {
                   return sfc::abs_diff(t1, t2);
                 });
  // Get the sum of the squares
  auto res = std::accumulate(
      std::begin(res_diff), std::end(res_diff), 0.0,
      [](const auto& t1, const auto& t2) { return t1 + std::pow(t2, 2.0); });

  // Return the sqrt of the sum of the squares
  return std::sqrt(res);
}

template <class T, sfc::size_t N>
T InfiniteNormTo(const coordinates<T, N>& coords1,
                 const coordinates<T, N>& coords2)
{
  // Get the absolute difference vector
  coordinates<T, N> res_diff;
  std::transform(std::begin(coords1), std::end(coords1), std::begin(coords2),
                 std::begin(res_diff), [](const auto& t1, const auto& t2) {
                   return sfc::abs_diff(t1, t2);
                 });
  //  Return the max value
  return *(std::max(std::begin(res_diff), std::end(res_diff)));
}

template <NORM norm, class T, sfc::size_t N>
auto DistanceTo(const coordinates<T, N>& coords1,
                const coordinates<T, N>& coords2)
    -> std::enable_if_t<norm == NORM::FIRST, T>
{
  return ManhattenDistanceTo(coords1, coords2);
}
template <NORM norm, class T, sfc::size_t N>
auto DistanceTo(const coordinates<T, N>& coords1,
                const coordinates<T, N>& coords2)
    -> std::enable_if_t<norm == NORM::SECOND, double>
{
  return EuclideanDistanceTo(coords1, coords2);
}

template <NORM norm, class T, sfc::size_t N>
auto DistanceTo(const coordinates<T, N>& coords1,
                const coordinates<T, N>& coords2)
    -> std::enable_if_t<norm == NORM::INFINITE, T>
{
  return InfiniteNormTo(coords1, coords2);
}

template <class T, sfc::size_t N>
constexpr T DistanceTo(const coordinates<T, N>& coords1,
                       const coordinates<T, N>& coords2, const NORM& norm)
{
  switch (norm) {
    case NORM::FIRST:
      return ManhattenDistanceTo(coords1, coords2);
    case NORM::SECOND:
      return EuclideanDistanceTo(coords1, coords2);
    case NORM::INFINITE:
      return InfiniteNormTo(coords1, coords2);
    default:
      throw std::exception();
  }
}
};  // namespace coords
};  // namespace sfc

template <class T, sfc::size_t N>
sfc::coordinates<T, N> operator+(const sfc::coordinates<T, N>& lhs,
                                 const sfc::coordinates<T, N>& rhs)
{
  auto output = lhs;
  for (auto i : sfc::range<sfc::size_t>(0, N)) output[i] += rhs[i];
  return output;
}

template <class T, sfc::size_t N>
sfc::coordinates<T, N>& operator+=(sfc::coordinates<T, N>& lhs,
                                   const sfc::coordinates<T, N>& rhs)
{
  for (auto i : sfc::range<sfc::size_t>(0, N)) lhs[i] += rhs[i];
  return lhs;
}

#endif