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
  using sfcurve_t = sfc::sfcurve<_NDim>;
  using sfcurve_ptr_t = std::unique_ptr<sfcurve_t>;
  using metric_t = _TpMetric;

  template <typename _TpSFC>
  metric(const _TpSFC& sfc) : _curve{std::make_unique<_TpSFC>(sfc)}
  {
    // Verify that _TpSFC inherits from sfc::sfcurve
    // _NDim is just a formality
    static_assert(std::is_base_of_v<sfc::sfcurve<_NDim>, _TpSFC>);
  }
  virtual ~metric() {}

  virtual _TpMetric calculate() = 0;

  const sfcurve_ptr_t& curve_ptr() const { return _curve; }

 private:
  sfcurve_ptr_t _curve;
};

template <sfc::size_t _NDim>
class bounds_metric
    : public metric<_NDim, std::tuple<sfc::coordinates<sfc::size_t, _NDim>,
                                      sfc::coordinates<sfc::size_t, _NDim>>>
{
 public:
  using coordinates_t = sfc::coordinates<sfc::size_t, _NDim>;
  using _TpMetric = std::tuple<coordinates_t, coordinates_t>;
  using metric_t = metric<_NDim, _TpMetric>;

  template <typename _TpSFC>
  bounds_metric(const _TpSFC& sfc) : metric_t(sfc){};
  virtual ~bounds_metric() override {}

  virtual _TpMetric calculate()
  {
    auto& curve = metric_t::curve_ptr();
    coordinates_t min_c{curve->dimensionLength(), curve->dimensionLength()};
    coordinates_t max_c{};
    for (auto dist : sfc::range(curve->totalElements())) {
      auto c = curve->distToCoords(dist);
      min_c = {std::min(min_c.at(0), c.at(0)), std::min(min_c.at(1), c.at(1))};
      max_c = {std::max(max_c.at(0), c.at(0)), std::max(max_c.at(1), c.at(1))};
    }
    return _TpMetric{min_c, max_c};
  }
};
};  // namespace sfc

#endif