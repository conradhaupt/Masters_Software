/**
 * @file metric_ratio.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-17
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFC_METRIC_RATIO_H
#define SFC_METRIC_RATIO_H

#include <omp.h>
#include <cmath>
#include <map>
#include <mutex>
#include <numeric>
#include "coordinates.h"
#include "metric.h"
#include "progress_bar.h"

namespace sfc {
template <sfc::size_t _NDim>
class metric_ratio : public sfc::metric<_NDim>
{
 public:
  using ratio_t = int;
  using nPairs_t = unsigned long long;
  using metric_t = std::map<ratio_t, nPairs_t>;

 private:
  sfc::NORM _norm;
  std::unique_ptr<metric_t> _results;
  unsigned long long _result_sidelength;

 public:
  metric_ratio(const sfc::NORM& distance_norm) : _norm(distance_norm) {}
  virtual ~metric_ratio() {}

  virtual void calculateFor(
      const std::unique_ptr<sfc::sfcurve<_NDim>>& curve) override
  {
    metric_t metric_dist;
    auto pg = sfc::cli::progressbar(curve->totalElements() *
                                    curve->totalElements() / 2);
#pragma omp parallel shared(pg)
    {
      metric_t _prv_metric_dist;
#pragma omp for private(_prv_metric_dist) schedule(static)
      for (auto point1_it = curve->begin(); point1_it < curve->end();
           point1_it++) {
        auto point1_it_distance = point1_it.distance();
        auto point1_it_coordinates = point1_it.coordinates();
        for (auto point2_it = std::next(point1_it); point2_it < curve->end();
             point2_it++) {
          pg.addProgress();
          auto ratio = static_cast<int>(
              1000.0 * (point2_it.distance() - point1_it_distance) /
              (sfc::coords::DistanceTo(point2_it.coordinates(),
                                       point1_it_coordinates, _norm)));
          if (_prv_metric_dist.count(ratio) == 0)
            _prv_metric_dist[ratio]++;
          else
            _prv_metric_dist.at(ratio)++;
        }
      }
#pragma omp critical
      for (auto n : _prv_metric_dist) {
        metric_dist[n.first] += n.second;
      }
    }
    _results = std::make_unique<metric_t>(metric_dist);
    _result_sidelength = curve->dimensionLength();
  }

  virtual bool writeTo(std::ostream& os)
  {
    if (_results == nullptr || _result_sidelength == 0ULL) return false;
    os << "Ratio\tRegions\tNormalised" << std::endl;
    auto nElems = sfc::pow(_result_sidelength, 2);
    auto nComb = std::tgammal(nElems) / (std::tgammal(nElems - 2.0) * 2.0);
    for (auto pair : *_results) {
      os << std::setprecision(std::numeric_limits<int>::max_digits10)
         //  << std::scientific
         << pair.first << "\t"
         << std::setprecision(
                std::numeric_limits<unsigned long long>::max_digits10)
         << pair.second << "\t"
         << std::setprecision(std::numeric_limits<double>::max_digits10)
         << std::scientific << (static_cast<double>(pair.second) / nComb)
         << std::endl;
    }
    return true;
  }
};  // namespace sfc
};  // namespace sfc

#endif /* METRIC_RATIO_H */
