#include "libhilbert.h"
#include <gtest/gtest.h>
#include "libsfc/coordinates.h"
#include "libsfc/hilbert_mapping.h"

namespace {

TEST_P(libhilbert_test_case, rangeIsInvertible)
{
  auto original_dist = distance;
  auto coords =
      sfc::libhilbert::getIntCoordFromHKey<2, sfc::coordinates<sfc::size_t, 2>>(
          dimensionSize, original_dist);
  auto calculated_dist =
      sfc::libhilbert::getHKeyFromIntCoord<2, sfc::coordinates<sfc::size_t, 2>>(
          dimensionSize, coords);
  EXPECT_EQ(original_dist, calculated_dist);
}

INSTANTIATE_TEST_CASE_P(sfc_libhilbert, libhilbert_test_case,
                        ::testing::Range<sfc::size_t>(0, 16384, 32));
};  // namespace
