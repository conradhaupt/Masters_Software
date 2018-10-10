/**
 * @brief contains code to manage bit-interleaving
 *
 * @file interleave.h
 * @author Conrad Haupt
 * @date 2018-07-03
 */

#ifndef SFC_INTERLEAVE_H
#define SFC_INTERLEAVE_H

#include <immintrin.h>
#include <climits>
#include <cmath>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include "sfcdef.h"

namespace sfc {

namespace internal {
template <::sfc::size_t _NDim, typename _TpMask>
constexpr _TpMask __interleave_mask_impl() {
  static_assert(_NDim > 0);
  _TpMask _ONE{1};
  const _TpMask _shifter{_NDim};
  _TpMask mask{0};
  while (_ONE != 0) {
    mask |= _ONE;
    _ONE <<= _shifter;
  }
  return mask;
}
};  // namespace internal

template <sfc::size_t _NDim, typename _TpMask>
constexpr _TpMask interleave_mask =
    internal::__interleave_mask_impl<_NDim, _TpMask>();

template <sfc::size_t _NDim, typename _TpDist>
constexpr _TpDist expand_coord(const _TpDist& coord, const sfc::size_t& iDim) {
  return _pdep_u64(coord, interleave_mask<_NDim, _TpDist> << iDim);
}

template <sfc::size_t _NDim, typename _TpDist, typename _TpCoord>
constexpr _TpCoord remove_coord(const _TpDist& dist, const sfc::size_t& iDim) {
  return _pext_u64(dist, interleave_mask<_NDim, _TpDist> << iDim);
}

/**
 * @brief templated function that returns the maximum value of _TpCoord to fit
 * _NDim many in _TpDist through bit-interleaving.
 *
 * Process:
 * First the number of bits in _TpDist for a single coordinate is calculated.
 * If that results in a higher-resolution number than _TpCoord can store, the
 * maximum value of _TpCoord is returned. If _TpCoord can store the full _TpDist
 * value, the aforementioned value is returned.
 *
 * @tparam _NDim number of dimensions/coordinates
 * @tparam _TpDist morton distance type
 * @tparam _TpCoord coordinate type
 */
// template <sfc::size_t _NDim, typename _TpDist, typename _TpCoord, _TpCoord
// _max> struct can_interleave
//     : public ::sfc::internal::__can_interleave_impl<
//           sizeof(_TpDist) * CHAR_BIT, ::sfc::log2e<_TpCoord, 0, _max>::value,
//           _NDim, void> {};

template <sfc::size_t _NDim, typename _TpDist, typename _TpCoord>
constexpr _TpCoord max_interleave_dim_length() {
  sfc::size_t nBitsDist{sizeof(_TpDist) * CHAR_BIT};
  sfc::size_t nBitsCoord{sizeof(_TpCoord) * CHAR_BIT};
  sfc::size_t nBitsCoord_fromDist{nBitsDist / _NDim};
  return (1ULL << std::min(nBitsCoord, nBitsCoord_fromDist)) - 1;
}

template <sfc::size_t _NDim, typename _TpDist, typename _TpCoords,
          typename _TpCoord>
constexpr _TpDist interleaveBits(const _TpCoords& coords) {
  if (!BMI2_SUPPORTED)
    throw std::runtime_error(
        "BMI2 is not supported, required for bit-interleaving");

  auto idx = 0;
  _TpDist dist{0};
  for (auto c : coords) {
    dist |= expand_coord<_NDim, _TpDist>(c, idx++);
  }
  return dist;
}

template <sfc::size_t _NDim, typename _TpDist, typename _TpCoords,
          typename _TpCoord>
constexpr _TpCoords uinterleaveBits(const _TpDist& dist) {
  if (!BMI2_SUPPORTED)
    throw std::runtime_error(
        "BMI2 is not supported, required for bit-interleaving");

  _TpCoords coords{};
  auto idx{0};
  for (auto& coord : coords) {
    coord = remove_coord<_NDim, _TpDist, _TpCoord>(dist, idx++);
  }

  return coords;
}
};  // namespace sfc

#endif