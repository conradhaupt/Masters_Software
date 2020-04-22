/**
 * @file metric_clusters.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-09
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFC_METRIC_CLUSTERS_H
#define SFC_METRICS_CLUSTERS_H

#include <exception>
#include <limits>
#include <map>
#include <memory>
#include "clusters.h"
#include "metric.h"
#include "progress_bar.h"
#include "raster.h"

namespace sfc {
template <sfc::size_t _NDim>
class clusters_metric : public sfc::metric<_NDim>
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
  using sidelength_t = unsigned long;
  using index_t = unsigned long long;
  const sidelength_t _region_sidelength;

 public:
  using metric_t = std::map<nClusters_t, nRegions_t>;
  std::unique_ptr<metric_t> _results;
  unsigned long long _result_sidelength;

  clusters_metric(const sidelength_t region_sidelength)
      : _region_sidelength(region_sidelength)
  {
    if (_region_sidelength == 0)
      throw std::invalid_argument(
          "region_sidelength must be a positive integer.");
  }

  virtual void calculateFor(const std::unique_ptr<sfc::sfcurve<_NDim>>& curve)
  {
    metric_t metric_dist;
    auto region_coord_curve =
        sfc::raster<_NDim>(curve->dimensionLength() - _region_sidelength + 1);

    const auto region_curve = sfc::raster<_NDim>(_region_sidelength);

    // For each region
    // auto _region_coord = std::begin(region_coord_curve);
    auto pg = sfc::cli::progressbar(region_coord_curve.totalElements());
    std::mutex map_mtx;
#pragma omp parallel for shared(pg)
    for (auto _region_coord = region_coord_curve.begin();
         _region_coord < region_coord_curve.end(); ++_region_coord) {
      pg.addProgress();
      sfc::clusters<index_t> _region_clusters;
      // For each point in the region
      for (const auto _region_offset : region_curve) {
        _region_clusters.insert(curve->coordsToDist((*_region_coord).coords +
                                                    _region_offset.coords));
      }
      map_mtx.lock();
      metric_dist[_region_clusters.count()]++;
      map_mtx.unlock();
    }

    // Save results
    _results = std::make_unique<metric_t>(metric_dist);
    _result_sidelength = curve->dimensionLength();
  }

  virtual bool writeTo(std::ostream& os)
  {
    if (_results == nullptr || _result_sidelength == 0ULL) return false;
    os << "Clusters\tRegions\tNormalised" << std::endl;
    for (auto pair : *_results) {
      os << std::setprecision(
                std::numeric_limits<unsigned long long>::max_digits10)
         << pair.first << "\t"
         << std::setprecision(
                std::numeric_limits<unsigned long long>::max_digits10)
         << pair.second << "\t"
         << std::setprecision(std::numeric_limits<double>::max_digits10)
         << std::scientific
         << (static_cast<double>(pair.second) /
             sfc::pow(_result_sidelength - _region_sidelength + 1, 2))
         << std::endl;
    }
    return true;
  }
};
};  // namespace sfc

#endif