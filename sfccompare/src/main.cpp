/**
 * @file main.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-05-23
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cmath>
#include <fstream>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include "libsfc/algorithm.h"
#include "libsfc/base2.h"
#include "libsfc/gray.h"
#include "libsfc/hilbert.h"
#include "libsfc/metric.h"
#include "libsfc/metric_bbox.h"
#include "libsfc/metric_clusters.h"
#include "libsfc/metric_gridedges.h"
#include "libsfc/metric_ratio.h"
#include "libsfc/morton.h"
#include "libsfc/range.h"
#include "libsfc/raster.h"

using namespace std::literals::string_literals;

void printHelp(const std::string programName)
{
  std::cout << programName
            << " <command> [start dimension length] <dimension length>    \n\n"
               "help                                                        \n"
               "  prints this help documentation                            \n"
               "cluster <sfc>                                               \n"
               "  calculates the cluster metric for the given sfc           \n"
               "ratio <sfc>                                                 \n"
               "  calculates the distance ratio metric for the given sfc    \n"
               "bbox <sfc>                                                  \n"
               "  calculates the bounding box metric for the given sfc      \n"
               "gridedges <sfc>                                             \n"
               "  calculates the grid-edges metric for the given sfc      \n\n";
}

enum class Metric
{
  CLUSTER,
  RATIO,
  BBOX,
  GRIDEDGES
};

std::optional<Metric> stringToMetric(const std::string& strMetric)
{
  if (strMetric == "cluster"s) {
    return Metric::CLUSTER;
  } else if (strMetric == "bbox"s) {
    return Metric::BBOX;
  } else if (strMetric == "ratio"s) {
    return Metric::RATIO;
  } else if (strMetric == "gridedges"s) {
    return Metric::GRIDEDGES;
  } else
    return {};
}

std::string metricToString(const Metric& _metric)
{
  switch (_metric) {
    case Metric::CLUSTER:
      return "cluster";
      // break;
    case Metric::RATIO:
      return "ratio";
      // break;
    case Metric::BBOX:
      return "bbox";
      // break;
    case Metric::GRIDEDGES:
      return "gridedges";
      // break;
    default:
      return "Null";
      // break;
  }
}

enum class SFC
{
  RASTER,
  MORTON,
  GRAY,
  HILBERT
};

std::optional<SFC> stringToSFC(const std::string& strSFC)
{
  if (strSFC == "raster"s) {
    return SFC::RASTER;
  } else if (strSFC == "morton"s) {
    return SFC::MORTON;
  } else if (strSFC == "gray"s) {
    return SFC::GRAY;
  } else if (strSFC == "hilbert"s) {
    return SFC::HILBERT;
  } else
    return {};
}

std::string SFCToString(const SFC& _sfc)
{
  switch (_sfc) {
    case SFC::RASTER:
      return "raster";
    case SFC::MORTON:
      return "morton";
    case SFC::GRAY:
      return "gray";
    case SFC::HILBERT:
      return "hilbert";
    default:
      return "Null";
  }
}

std::unique_ptr<sfc::sfcurve<2>> SFCToSFCurve(const SFC& _sfc,
                                              sfc::size_t dimLength)
{
  switch (_sfc) {
    case SFC::RASTER:
      return std::make_unique<sfc::raster<2>>(dimLength);
    case SFC::MORTON:
      return std::make_unique<sfc::morton<2>>(dimLength);
    case SFC::GRAY:
      return std::make_unique<sfc::gray<2>>(dimLength);
    case SFC::HILBERT:
      return std::make_unique<sfc::hilbert<2>>(dimLength);
    default:
      return nullptr;
  }
}

std::unique_ptr<sfc::metric<2>> MetricToMetricOBJ(const Metric& _metric)
{
  switch (_metric) {
    case Metric::CLUSTER:
      return std::make_unique<sfc::clusters_metric<2>>(4ULL);
    case Metric::RATIO:
      return std::make_unique<sfc::metric_ratio<2>>(sfc::NORM::SECOND);
    case Metric::BBOX:
      return std::make_unique<sfc::bounds_metric<2>>();
    case Metric::GRIDEDGES:
      return std::make_unique<sfc::metric_gridedges<2>>();
    default:
      return nullptr;
  }
}

int main(int argc, char** argv)
{
  // This is required to use ""s std::string literals
  const std::string _programName = argv[0];
  if (argc != 4 && argc != 5) {
    std::cout << "Insufficient arguments" << std::endl;
    printHelp(_programName);
    return -1;
  }
  const std::string _command = argv[1];
  Metric _metric;
  if (_command == "help"s || argc == 2) {
    printHelp(_programName);
    return 0;
  } else if (stringToMetric(_command)) {
    _metric = stringToMetric(_command).value();
  } else {
    std::cout << "Invalid arguments" << std::endl;
    printHelp(_programName);
    return -1;
  }

  unsigned long long _cli_length;
  unsigned long long _cli_min_length;
  if (argc == 5) {
    _cli_length = std::stoull(argv[4]);
    _cli_min_length = std::stoull(argv[3]);
  } else {
    _cli_length = std::stoull(argv[3]);
    _cli_min_length = _cli_length;
  }

  if (_cli_min_length < 4) {
    std::cerr << "Length is too small. Must be >=4" << std::endl;
    return -1;
  }

  const unsigned long long dimLength_exponent =
      static_cast<unsigned long long>(std::ceil(std::log2(_cli_length)));
  const unsigned long long minDimLength_exponent =
      static_cast<unsigned long long>(std::ceil(std::log2(_cli_min_length)));
  const std::string _sfc = argv[2];
  if (!stringToSFC(_sfc)) {
    std::cout << "Invalid SFC name" << std::endl;
    return -1;
  }
  for (auto k = minDimLength_exponent; k <= dimLength_exponent; k++) {
    const unsigned long long dimLength = sfc::pow(2, k);
    auto sfc = SFCToSFCurve(stringToSFC(_sfc).value(), dimLength);
    auto sfc_name = SFCToString(stringToSFC(_sfc).value());

    std::cout << "You chose " << sfc_name << " of " << dimLength << "x"
              << dimLength << " with the " << metricToString(_metric)
              << " metric" << std::endl;
    const auto sidelength = dimLength;
    auto mtrc_clusters = MetricToMetricOBJ(_metric);
    mtrc_clusters->calculateFor(sfc);
    std::ofstream mtrc_out;
    mtrc_out.open("mtrc_" + metricToString(_metric) + "_" + sfc_name + "_" +
                  std::to_string(sidelength) + ".dat");
    if (!mtrc_clusters->writeTo(mtrc_out))
      std::cout << "Clusters Metric failed to write to file!" << std::endl;
    mtrc_out.close();
  }

  return 0;
}
