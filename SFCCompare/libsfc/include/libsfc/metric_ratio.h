/**
 * @file metric_ratio.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-05-17
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SFC_METRIC_RATIO_H
#define SFC_METRIC_RATIO_H

#include <map>
#include <numeric>
#include "coordinates.h"
#include "metric.h"

namespace sfc {
template <sfc::size_t _NDim>
class metric_ratio : public sfc::metric<_NDim, std::map<double, unsigned long>>
{
 public:
  using ratio_t = double;
  using nPairs_t = unsigned long;

 private:
  using _metric_inherit = sfc::metric<_NDim, std::map<ratio_t, nPairs_t>>;
  sfc::NORM _norm;

 public:
  using metric_t = typename _metric_inherit::metric_t;

  template <typename _TpSFC>
  metric_ratio(const _TpSFC& sfc, const sfc::NORM& distance_norm)
      : _metric_inherit::metric(sfc), _norm(distance_norm)
  {}

  virtual metric_t calculate()
  {
    metric_t metric_dist;
    for (auto point1_it = this->curve_ptr()->begin();
         point1_it < this->curve_ptr()->end(); point1_it++) {
      for (auto point2_it = std::next(point1_it);
           point1_it < point2_it && point2_it < this->curve_ptr()->end();
           point2_it++) {
        auto ratio =
            (static_cast<double>(point2_it.distance() - point1_it.distance())) /
            (sfc::coords::DistanceTo(point2_it.coordinates(), point1_it.coordinates(), _norm));
        metric_dist[ratio]++;
      }
    }
    return metric_dist;
  }
};
};  // namespace sfc

#endif /* METRIC_RATIO_H */
