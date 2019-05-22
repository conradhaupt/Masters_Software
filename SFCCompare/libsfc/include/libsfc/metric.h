/**
 * @file metric.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-10-08
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef SFC_METRIC_H
#define SFC_METRIC_H

#include <memory>
#include "coordinates.h"
#include "sfcdef.h"
#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim, typename _TpMetric>
class metric
{
 public:
  using metric_t = _TpMetric;

  metric() {}
  virtual ~metric() {}
  constexpr sfc::size_t ndim() const { return _NDim; }

  virtual metric_t calculateFor(const sfc::sfcurve<_NDim>& curve) = 0;
};

template <sfc::size_t _NDim>
class bounds_metric
    : public metric<_NDim, std::tuple<sfc::coordinates<sfc::size_t, _NDim>,
                                      sfc::coordinates<sfc::size_t, _NDim>>>
{
 public:
  using coordinates_t = sfc::coordinates<sfc::size_t, _NDim>;
  using metric_inherit =
      metric<_NDim, std::tuple<sfc::coordinates<sfc::size_t, _NDim>,
                               sfc::coordinates<sfc::size_t, _NDim>>>;
  using metric_t = typename metric_inherit::metric_t;

  bounds_metric() {}
  virtual ~bounds_metric() override {}

  virtual metric_t calculateFor(const sfc::sfcurve<_NDim>& curve) override
  {
    coordinates_t min_c{curve.dimensionLength(), curve.dimensionLength()};
    coordinates_t max_c{};
    for (auto dist : sfc::range(curve.totalElements())) {
      auto c = curve.distToCoords(dist);
      min_c = {std::min(min_c.at(0), c.at(0)), std::min(min_c.at(1), c.at(1))};
      max_c = {std::max(max_c.at(0), c.at(0)), std::max(max_c.at(1), c.at(1))};
    }
    return metric_t{min_c, max_c};
  }
};
};  // namespace sfc

#endif