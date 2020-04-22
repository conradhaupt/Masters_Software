/**
 * @file metric.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFC_METRIC_H
#define SFC_METRIC_H

#include <memory>
#include <ostream>
#include "coordinates.h"
#include "sfcdef.h"
#include "sfcurve.h"

namespace sfc {
template <sfc::size_t _NDim>
class metric
{
 public:
  metric() {}
  virtual ~metric() {}
  constexpr sfc::size_t ndim() const { return _NDim; }

  virtual void calculateFor(
      const std::unique_ptr<sfc::sfcurve<_NDim>>& curve) = 0;

  virtual bool writeTo(std::ostream& os) = 0;
};
};  // namespace sfc

#endif