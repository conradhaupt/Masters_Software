/**
 * @file hilbert_mapping.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-21
 *
 * @copyright Copyright (c) 2020
 *
 * Used to map the C library libhilbert to C++. This code uses the modified
 * version hosted at https://github.com/conradhaupt/libhilbert which was forked
 * from https://github.com/xkfengxd227/libhilbert.
 *
 */
#ifndef SFC_HILBERT_MAPPING_H
#define SFC_HILBERT_MAPPING_H

#include <algorithm>
#include <exception>
#include "base2.h"
#include "sfcdef.h"

namespace sfc {
namespace __extern_c_libhilbert {
extern "C" {
#include <hilbertKey.h>
};
};  // namespace __extern_c_libhilbert

namespace libhilbert {
template <sfc::size_t _NDim, typename coords_type>
uint64_t getHKeyFromIntCoord(const sfc::size_t dimSize,
                             const coords_type& coords)
{
  // Coords type must match the number of dimensions
  static_assert(coords.size() == _NDim);
  uint64_t tmp_coords[_NDim];
  std::copy(std::begin(coords), std::end(coords), std::rbegin(tmp_coords));
  // std::for_each(std::begin(coords), std::end(coords),
  //               [](auto& i) { std::cout << i << " "; });
  // std::cout << std::endl;
  // std::for_each(std::begin(tmp_coords), std::end(tmp_coords),
  //               [](auto& i) { std::cout << i << " "; });
  // std::cout << std::endl;
  int error;
  auto dim_m = sfc::ilog2(dimSize);
  auto hKey = __extern_c_libhilbert::getHKeyFromIntCoord(dim_m, _NDim,
                                                         tmp_coords, &error);
  if (error == HKEY_ERR_DIM) {
    throw std::out_of_range("HKEY_ERR_DIM");
  } else if (error == HKEY_ERR_NOMEM) {
    throw std::bad_alloc();
  } else if (error == HKEY_ERR_OK) {
    return hKey;
  } else {
    throw std::exception();
  }
}

template <sfc::size_t _NDim, typename coords_type>
coords_type getIntCoordFromHKey(const sfc::size_t dimSize,
                                const uint64_t& distance)
{
  // Coords type must match the number of dimensions
  uint64_t tmp_coords[_NDim];
  int error;
  auto dim_m = sfc::ilog2(dimSize);
  __extern_c_libhilbert::getIntCoordFromHKey(tmp_coords, dim_m, _NDim, distance,
                                             &error);
  if (error == HKEY_ERR_DIM) {
    throw std::out_of_range("HKEY_ERR_DIM");
  } else if (error == HKEY_ERR_NOMEM) {
    throw std::bad_alloc();
  } else if (error != HKEY_ERR_OK) {
    throw std::exception();
  }

  coords_type coords;
  std::copy(std::begin(tmp_coords), std::end(tmp_coords), std::rbegin(coords));
  return coords;
}

};  // namespace libhilbert
};  // namespace sfc
#endif