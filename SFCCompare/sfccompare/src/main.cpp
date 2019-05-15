//
// Created by conrad on 2018/05/23.
//

#include <fstream>
#include <initializer_list>
#include <map>
#include "libsfc/algorithm.h"
#include "libsfc/gray.h"
#include "libsfc/hilbert.h"
#include "libsfc/metric.h"
#include "libsfc/metric_clusters.h"
#include "libsfc/morton.h"
#include "libsfc/range.h"
#include "libsfc/raster.h"

int main(void)
{
  constexpr ::sfc::size_t DIM_SIZE = 1UL << 3;
  constexpr ::sfc::size_t DIM_COUNT = 2;

  // MORTON
  sfc::morton<DIM_COUNT> mrtn(DIM_SIZE);
  std::ofstream mortonFile;
  mortonFile.open("./morton.coords");
  auto rows = 0;
  auto newline = 0;
  for (auto dist : sfc::range<sfc::size_t>(DIM_SIZE * DIM_SIZE)) {
    auto coords = mrtn.distToCoords(dist);
    if ((dist / DIM_SIZE) != rows) {
      rows = dist / DIM_SIZE;
      std::cout << ".";
    }
    if ((dist / (DIM_SIZE * 16)) != newline) {
      newline = dist / (DIM_SIZE * 16);
      std::cout << newline << std::endl;
    }
    mortonFile << coords[0] << "\t" << coords[1] << std::endl;
  }
  mortonFile.close();

  // HILBERT
  sfc::hilbert<DIM_COUNT> hlbrt(DIM_SIZE);
  std::ofstream hilbertFile;
  hilbertFile.open("./hilbert.coords");
  rows = 0;
  newline = 0;
  for (auto dist : sfc::range<sfc::size_t>(DIM_SIZE * DIM_SIZE)) {
    auto coords = hlbrt.distToCoords(dist);
    if ((dist / DIM_SIZE) != rows) {
      rows = dist / DIM_SIZE;
      std::cout << ".";
    }
    if ((dist / (DIM_SIZE * 16)) != newline) {
      newline = dist / (DIM_SIZE * 16);
      std::cout << newline << std::endl;
    }
    hilbertFile << coords[0] << "\t" << coords[1] << std::endl;
  }
  hilbertFile.close();

  // GRAY
  sfc::gray<DIM_COUNT> gry(DIM_SIZE);
  std::ofstream grayFile;
  grayFile.open("./gray.coords");
  rows = 0;
  newline = 0;
  for (auto dist : sfc::range<sfc::size_t>(DIM_SIZE * DIM_SIZE)) {
    auto coords = gry.distToCoords(dist);
    if ((dist / DIM_SIZE) != rows) {
      rows = dist / DIM_SIZE;
      std::cout << ".";
    }
    if ((dist / (DIM_SIZE * 16)) != newline) {
      newline = dist / (DIM_SIZE * 16);
      std::cout << newline << std::endl;
    }
    grayFile << coords[0] << "\t" << coords[1] << std::endl;
  }
  grayFile.close();

  // RASTER
  sfc::raster<DIM_COUNT> rstr(DIM_SIZE);
  std::ofstream rasterFile;
  rasterFile.open("./raster.coords");
  rows = 0;
  newline = 0;
  for (auto dist : sfc::range<sfc::size_t>(DIM_SIZE * DIM_SIZE)) {
    auto coords = rstr.distToCoords(dist);
    if ((dist / DIM_SIZE) != rows) {
      rows = dist / DIM_SIZE;
      std::cout << ".";
    }
    if ((dist / (DIM_SIZE * 16)) != newline) {
      newline = dist / (DIM_SIZE * 16);
      std::cout << newline << std::endl;
    }
    rasterFile << coords[0] << "\t" << coords[1] << std::endl;
  }
  rasterFile.close();

  auto mtrc = sfc::bounds_metric<2>(gry);
  auto mtrc_res = mtrc.calculate();
  std::cout << std::get<0>(std::get<0>(mtrc_res)) << ", "
            << std::get<1>(std::get<0>(mtrc_res)) << " - "
            << std::get<0>(std::get<1>(mtrc_res)) << ", "
            << std::get<1>(std::get<1>(mtrc_res)) << std::endl;

  auto mtrc_clstrs = sfc::clusters_metric<2>(mrtn, 2);
  auto result = mtrc_clstrs.calculate();

  for (auto i : result) {
    std::cout << "(" << i.first << ") " << i.second << std::endl;
  }
  auto mtrc_clstrs_num_regions =
      std::accumulate(std::begin(result), std::end(result), 0LL,
                      [](auto c, auto e) { return c + e.second; });
  auto mtrc_clstrs_average =
      std::accumulate(
          std::begin(result), std::end(result), 0.0,
          [](auto curr, auto e) { return curr + (e.first * e.second); }) /
      mtrc_clstrs_num_regions;
  auto mtrc_clstrs_max =
      std::max_element(std::begin(result), std::end(result),
                       [](auto a, auto b) { return a.first < b.first; });
  std::cout << "Average number of clusters is " << mtrc_clstrs_average
            << std::endl;
  std::cout << "Maximum number of clusters is " << mtrc_clstrs_max->first
            << std::endl;
  return 0;
}
