#include "libsfc/sfcurve.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <tuple>
#include "libsfc/sfcdef.h"
#include "sfcurve_test.h"

using index_type = typename sfc::sfcurve<2>::index_type;
using coords_type = typename sfc::sfcurve<2>::coords_type;
using dist_type = typename sfc::sfcurve<2>::dist_type;
class simplecurve : public sfc::sfcurve<2> {
 public:
  using sfc::sfcurve<2>::sfcurve;
  virtual size_type coordsToDist(const coords_type &coords) {
    return sfc::sfcurve<2>::coordsToIndex(coords);
  };
  virtual coords_type distToCoords(const dist_type &dist) {
    return sfc::sfcurve<2>::indexToCoords(dist);
  };
};

TEST_P(sfcurve_test_case, coordMapMatchesIndexMap) {
  simplecurve sc(dimLength);
  auto index = sc.coordsToDist(coordinates);
  auto returnedCoordinates = sc.distToCoords(index);
  EXPECT_EQ(coordinates, returnedCoordinates);
}

INSTANTIATE_TEST_CASE_P(
    sfc_sfcurve, sfcurve_test_case,
    ::testing::Combine(::testing::Range<double>(0.0, 1.0, 0.2),
                       ::testing::Range<double>(0.0, 1.0, 0.2),
                       ::testing::Range<index_type>(4, 32, 4)));
