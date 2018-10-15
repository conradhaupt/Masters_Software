#include "libsfc/sfcurve.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <tuple>
#include "libsfc/sfcdef.h"
#include "sfcurve_test.h"
#include "simple_curve.h"

TEST_P(sfcurve_test_case, coordMapMatchesIndexMap)
{
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
