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
};  // namespace sfc

#endif