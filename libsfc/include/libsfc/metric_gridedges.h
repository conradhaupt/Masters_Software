/**
 * @file metric_gridedges.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-25
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef METRIC_GRIDEDGES_H
#define METRIC_GRIDEDGES_H

#include <cmath>
#include <functional>
#include <map>
#include "coordinates.h"
#include "metric.h"
#include "progress_bar.h"
#include "raster.h"
#include "sfcdef.h"
namespace sfc {
template <sfc::size_t _NDim>
class metric_gridedges : public metric<_NDim>
{
 public:
  using coordinates_t = typename sfc::coordinates<sfc::size_t, _NDim>;
  using metric_t = std::map<unsigned long long, unsigned long long>;

 private:
  std::unique_ptr<metric_t> _results;
  unsigned long long _result_sidelength;

 public:
  metric_gridedges() {}
  virtual ~metric_gridedges() override {}

  virtual void calculateFor(
      const std::unique_ptr<sfc::sfcurve<_NDim>>& curve) override
  {
    metric_t metric_dist;
    auto pg = sfc::cli::progressbar(curve->totalElements() *
                                    curve->totalElements() / 2);
    std::mutex metric_mutex;
    auto _traversal_curve = sfc::raster<_NDim>(curve->dimensionLength() - 1);
#pragma omp parallel for shared(pg)
    for (auto point = std::begin(_traversal_curve);
         point < std::end(_traversal_curve); point++) {
      pg.addProgress();
      auto point_coords = point.coordinates();
      for (auto ni = 0; ni < _NDim; ni++) {
        if (point_coords[ni] != 0) {
          auto neg_dist = sfc::abs_diff(
              curve->coordsToDist(
                  point_coords -
                  sfc::coords::make_unitcoordinate<unsigned long, _NDim>(ni)),
              point.distance());
          metric_mutex.lock();
          metric_dist[neg_dist]++;
          metric_mutex.unlock();
        }
        auto dist = sfc::abs_diff(
            curve->coordsToDist(
                point_coords +
                sfc::coords::make_unitcoordinate<unsigned long, _NDim>(ni)),
            point.distance());
        metric_mutex.lock();
        metric_dist[dist]++;
        metric_mutex.unlock();
      }
    }
    _results = std::make_unique<metric_t>(metric_dist);
    _result_sidelength = curve->dimensionLength();
  }

  virtual bool writeTo(std::ostream& os)
  {
    if (_results == nullptr || _result_sidelength == 0ULL) return false;
    auto nElems = sfc::pow(_result_sidelength, 2);
    auto nComb = std::tgammal(nElems) / (std::tgammal(nElems - 2.0) * 2.0);
    os << "Distance\tRegions\tNormalised" << std::endl;
    for (auto pair : *_results) {
      os << std::setprecision(
                std::numeric_limits<unsigned long long>::max_digits10)
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
};
};  // namespace sfc

#endif /* METRIC_GRIDEDGES_H */
