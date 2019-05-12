#ifndef SFC_TEST_SIMPLE_CURVE_H
#define SFC_TEST_SIMPLE_CURVE_H

#include "libsfc/sfcurve.h"

using index_type = typename sfc::sfcurve<2>::index_type;
using coords_type = typename sfc::sfcurve<2>::coords_type;
using dist_type = typename sfc::sfcurve<2>::dist_type;
class simplecurve : public sfc::sfcurve<2>
{
 public:
  using sfc::sfcurve<2>::sfcurve;
  virtual dist_type coordsToDist(const coords_type &coords) const
  {
    return sfc::sfcurve<2>::coordsToIndex(coords);
  };
  virtual coords_type distToCoords(const dist_type &dist) const
  {
    return sfc::sfcurve<2>::indexToCoords(dist);
  };
  std::unique_ptr<sfc::sfcurve<2>> clone() const override
  {
    return std::make_unique<simplecurve>(*this);
  }
};
#endif