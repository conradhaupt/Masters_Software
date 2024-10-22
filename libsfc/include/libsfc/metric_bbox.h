/**
 * @file metric_bbox.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-22
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFC_METRIC_BBOX_H
#define SFC_METRIC_BBOX_H
#include <cmath>
#include <functional>
#include <map>
#include "metric.h"
#include "progress_bar.h"
#include "rectangle.h"
#include "sfcdef.h"
namespace sfc {
template <sfc::size_t _NDim>
class bounds_metric : public metric<_NDim>
{
 public:
  using coordinates_t = typename sfc::coordinates<sfc::size_t, _NDim>;
  using metric_t = std::map<double, unsigned long long>;

 private:
  std::unique_ptr<metric_t> _results;
  unsigned long long _result_sidelength;

 public:
  bounds_metric() {}
  virtual ~bounds_metric() override {}

  virtual void calculateFor(
      const std::unique_ptr<sfc::sfcurve<_NDim>>& curve) override
  {
    metric_t metric_dist;
    auto pg = sfc::cli::progressbar(curve->totalElements() *
                                    curve->totalElements() / 2);
    std::mutex metric_mutex;
#pragma omp parallel for shared(pg)
    for (auto point1_it = curve->begin(); point1_it < curve->end();
         point1_it++) {
      sfc::rectangle<sfc::size_t, _NDim> _region(point1_it.coordinates(),
                                                 point1_it.coordinates());
      for (auto point2_it = std::next(point1_it); point2_it < curve->end();
           point2_it++) {
        pg.addProgress();
        _region.expandToInclude(point2_it.coordinates());

        // std::cout << std::scientific << area << "\t" << point2_it.distance()
        //           << "\t" << point1_it.distance() << std::endl;
        auto wba =
            static_cast<double>(_region.area()) / (point2_it - point1_it);
        // Calculate area
        metric_mutex.lock();
        metric_dist[wba]++;
        metric_mutex.unlock();
      }
    }
    _results = std::make_unique<metric_t>(metric_dist);
    _result_sidelength = curve->dimensionLength();
  }

  virtual bool writeTo(std::ostream& os)
  {
    if (_results == nullptr || _result_sidelength == 0ULL) return false;
    os << "Area\tRegions\tNormalised" << std::endl;
    auto nElems = sfc::pow(_result_sidelength, 2);
    auto nComb = std::tgammal(nElems) / (std::tgammal(nElems - 2.0) * 2.0);
    for (auto pair : *_results) {
      os << std::setprecision(std::numeric_limits<double>::max_digits10)
         << std::scientific << pair.first << "\t"
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
};     // namespace sfc
#endif /* METRIC_BBOX_H */
