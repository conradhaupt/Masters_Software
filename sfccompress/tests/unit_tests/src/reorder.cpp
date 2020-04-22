/**
 * @file reorder.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "reorder.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include "libsfc/gray.h"
#include "libsfc/hilbert.h"
#include "libsfc/morton.h"
#include "libsfc/raster.h"
#include "reorder_test.h"

typedef testing::Types<SFCurveTypes<sfc::raster<2>, sfc::morton<2>>,
                       SFCurveTypes<sfc::raster<2>, sfc::gray<2>>,
                       SFCurveTypes<sfc::raster<2>, sfc::hilbert<2>>,
                       SFCurveTypes<sfc::morton<2>, sfc::raster<2>>,
                       SFCurveTypes<sfc::morton<2>, sfc::gray<2>>,
                       SFCurveTypes<sfc::morton<2>, sfc::hilbert<2>>,
                       SFCurveTypes<sfc::gray<2>, sfc::raster<2>>,
                       SFCurveTypes<sfc::gray<2>, sfc::morton<2>>,
                       SFCurveTypes<sfc::gray<2>, sfc::hilbert<2>>,
                       SFCurveTypes<sfc::hilbert<2>, sfc::raster<2>>,
                       SFCurveTypes<sfc::hilbert<2>, sfc::morton<2>>,
                       SFCurveTypes<sfc::hilbert<2>, sfc::gray<2>>>
    ReorderTypes;
TYPED_TEST_SUITE(reorder_test_case, ReorderTypes);

TYPED_TEST(reorder_test_case, forwardMappingWorks)
{
  auto size = 128;
  auto totalsize = size * size;
  auto reorderer =
      sfcc::reorderer(std::make_unique<typename TypeParam::from_t>(size),
                      std::make_unique<typename TypeParam::to_t>(size));
  std::unique_ptr<int[]> array = std::make_unique<int[]>(totalsize);
  std::unique_ptr<int[]> expected = std::make_unique<int[]>(totalsize);
  auto n = 0;
  std::generate(array.get(), array.get() + totalsize, [&n]() { return n++; });

  // Manually calculate
  auto from = std::make_unique<typename TypeParam::from_t>(size);
  auto to = std::make_unique<typename TypeParam::to_t>(size);
  for (auto i = 0; i < totalsize; i++)
    expected[i] = array[from->coordsToDist(to->distToCoords(i))];
  // expected[to->coordsToDist(from->distToCoords(i))] = array[i];

  // Run reorderer
  reorderer.reorder_withtemporary(array.get(), totalsize);
  for (auto i = 0; i < totalsize; i++) EXPECT_EQ(array[i], expected[i]);
}

TYPED_TEST(reorder_test_case, reorderingIsReversible)
{
  auto size = 128;
  auto totalsize = size * size;
  auto reorderer =
      sfcc::reorderer(std::make_unique<typename TypeParam::from_t>(size),
                      std::make_unique<typename TypeParam::to_t>(size));
  auto reverser =
      sfcc::reorderer(std::make_unique<typename TypeParam::to_t>(size),
                      std::make_unique<typename TypeParam::from_t>(size));
  std::unique_ptr<int[]> array = std::make_unique<int[]>(totalsize);
  std::unique_ptr<int[]> original = std::make_unique<int[]>(totalsize);
  auto n = 0;
  std::generate(array.get(), array.get() + totalsize, [&n]() { return n++; });
  std::copy(array.get(), array.get() + totalsize, original.get());

  // Run reorderer
  reorderer.reorder_withtemporary(array.get(), totalsize);
  reverser.reorder_withtemporary(array.get(), totalsize);
  for (auto i = 0; i < totalsize; i++) EXPECT_EQ(array[i], original[i]);
}

TEST_P(reorder_param_test_case, reorderingIsReversible)
{
  auto size = 128;
  auto totalsize = size * size;
  auto reorderer = sfcc::make_reorderer(2, size, from_type, to_type);
  auto reverser = sfcc::make_reorderer(2, size, to_type, from_type);
  std::unique_ptr<int[]> array = std::make_unique<int[]>(totalsize);
  std::unique_ptr<int[]> original = std::make_unique<int[]>(totalsize);
  auto n = 0;
  std::generate(array.get(), array.get() + totalsize, [&n]() { return n++; });
  std::copy(array.get(), array.get() + totalsize, original.get());

  // Run reorderer
  reorderer->reorder_withtemporary(array.get(), totalsize);
  reverser->reorder_withtemporary(array.get(), totalsize);
  for (auto i = 0; i < totalsize; i++) EXPECT_EQ(array[i], original[i]);
}
INSTANTIATE_TEST_CASE_P(
    reorder_param_tests, reorder_param_test_case,
    ::testing::Combine(::testing::Values(sfc::main::sfcs::types::ROW_MAJOR,
                                         sfc::main::sfcs::types::MORTON,
                                         sfc::main::sfcs::types::GRAY_CODE,
                                         sfc::main::sfcs::types::HILBERT),
                       ::testing::Values(sfc::main::sfcs::types::ROW_MAJOR,
                                         sfc::main::sfcs::types::MORTON,
                                         sfc::main::sfcs::types::GRAY_CODE,
                                         sfc::main::sfcs::types::HILBERT)));
