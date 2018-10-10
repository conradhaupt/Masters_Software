//
// Created by conrad on 2018/05/23.
//

#include <fstream>
#include "libsfc/morton.h"
#include "libsfc/range.h"

int main(void) {
  std::cout << std::endl;
  constexpr ::sfc::size_t DIM_SIZE = 1UL << 4;
  constexpr ::sfc::size_t DIM_COUNT = 2;
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
  return 0;
}
