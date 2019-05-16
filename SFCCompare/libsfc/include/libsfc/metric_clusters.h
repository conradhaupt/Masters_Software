/**
 * @file metric_clusters.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-05-09
 *
 * @copyright Copyright (c) 2019
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 *
 */
#ifndef SFC_METRIC_CLUSTERS_H
#define SFC_METRICS_CLUSTERS_H

#include <exception>
#include <map>
#include "clusters.h"
#include "metric.h"
#include "raster.h"

namespace sfc {
template <sfc::size_t _NDim>
class clusters_metric
    : public sfc::metric<_NDim, std::map<unsigned long, unsigned long>>
{
 public:
  // The metric for the Cluster-Count requires knowing how many regions have a
  // certain number of clusters. This is calculated using a map where the key is
  // the number of clusters and the value is the number of regions found
  // containing the given number of clusters. This class inherits the metric_t
  // memember type.
  using nClusters_t = unsigned long;
  using nRegions_t = unsigned long;

 private:
  using _metric_inherit = sfc::metric<_NDim, std::map<nClusters_t, nRegions_t>>;
  const typename sfc::sfcurve<_NDim>::index_type _region_sidelength;
  using sfcurve_t = typename _metric_inherit::sfcurve_t;

 public:
  using typename _metric_inherit::curve_ptr;
  using metric_t = typename _metric_inherit::metric_t;

  template <typename _TpSFC>
  clusters_metric(const _TpSFC& sfc,
                  const typename _TpSFC::index_type& region_sidelength)
      : _metric_inherit::metric(sfc), _region_sidelength(region_sidelength)
  {
    if (_region_sidelength == 0)
      throw std::invalid_argument(
          "region_sidelength must be a positive integer.");
  }

  virtual metric_t calculate()
  {
    metric_t metric_dist;
    auto region_coord_curve =
        sfc::raster<_NDim>(_metric_inherit::curve_ptr()->dimensionLength() -
                           _region_sidelength + 1);

    const auto region_curve = sfc::raster<_NDim>(_region_sidelength);

    // For each region
    // auto _region_coord = std::begin(region_coord_curve);
    std::mutex map_mtx;
#pragma omp parallel for shared(pg)
    for (auto _region_coord = region_coord_curve.begin();
         _region_coord < region_coord_curve.end(); ++_region_coord) {
      sfc::clusters<typename sfcurve_t::dist_type> _region_clusters;
      // For each point in the region
      for (const auto _region_offset : region_curve) {
        _region_clusters.insert(_metric_inherit::curve_ptr()->coordsToDist(
            (*_region_coord).coords + _region_offset.coords));
      }
      map_mtx.lock();
      metric_dist[_region_clusters.count()]++;
      map_mtx.unlock();
    }
    return metric_dist;
  }
};
};  // namespace sfc

#endif