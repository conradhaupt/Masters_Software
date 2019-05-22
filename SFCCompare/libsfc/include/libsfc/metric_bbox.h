/**
 * @file metric_bbox.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-05-22
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SFC_METRIC_BBOX_H
#define SFC_METRIC_BBOX_H
#include "metric.h"
#include "sfcdef.h"
namespace sfc {
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
};     // namespace sfc
#endif /* METRIC_BBOX_H */
