//
// Created by conrad on 2018/05/23.
//

#include <fstream>
#include "libsfc/gray.h"
#include "libsfc/hilbert.h"
#include "libsfc/morton.h"
#include "libsfc/range.h"

int main(void) {
  std::cout << std::endl;
  constexpr ::sfc::size_t DIM_SIZE = 1UL << 4;
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

  auto mtrc = sfc::bounds_metric<2>(gry);
  auto mtrc_res = mtrc.calculate();
  std::cout << std::get<0>(mtrc_res) << " - " << std::get<1>(mtrc_res) << std::endl;

  return 0;
}