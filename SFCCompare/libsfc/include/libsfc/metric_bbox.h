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
#include <functional>
#include <map>
#include "metric.h"
#include "progress_bar.h"
#include "sfcdef.h"
namespace sfc {
template <sfc::size_t _NDim>
class bounds_metric : public metric<_NDim, std::map<double, unsigned long long>>
{
 public:
  using coordinates_t = sfc::coordinates<sfc::size_t, _NDim>;
  using metric_inherit = metric<_NDim, std::map<double, unsigned long long>>;
  using metric_t = typename metric_inherit::metric_t;

  bounds_metric() {}
  virtual ~bounds_metric() override {}

  virtual metric_t calculateFor(const sfc::sfcurve<_NDim> &curve) override
  {
    metric_t metric_dist;
    auto pg = sfc::cli::progressbar(curve.totalElements() *
                                    curve.totalElements() / 2);
    std::mutex metric_mutex;
#pragma omp parallel for
    for (auto point1_it = std::begin(curve); point1_it < std::end(curve);
         point1_it++) {
      for (auto point2_it = std::next(point1_it);
           point1_it < point2_it && point2_it < std::end(curve); point2_it++) {
        pg.addProgress();
        //  Find maximum for traversal between point1 and point2
        sfc::coordinates<sfc::size_t, _NDim> bbox;
        for (auto ni = 0; ni < _NDim; ni++) {
          auto minmax = std::minmax_element(
              point1_it, std::next(point2_it), [ni](auto p1, auto p2) {
                return (p1.coords[ni] < p2.coords[ni]);
              });
          bbox[ni] = minmax.second - minmax.first;
        }
        auto area = std::accumulate(std::begin(bbox), std::end(bbox), 1.0,
                                    std::multiplies<double>());
        // std::cout << std::scientific << area << "\t" << point2_it.distance()
        //           << "\t" << point1_it.distance() << std::endl;
        auto wba = static_cast<double>(area) / (point2_it - point1_it);
        // Calculate area
        metric_mutex.lock();
        metric_dist[wba]++;
        metric_mutex.unlock();
      }
    }
    return metric_dist;
  }
};
};     // namespace sfc
#endif /* METRIC_BBOX_H */
