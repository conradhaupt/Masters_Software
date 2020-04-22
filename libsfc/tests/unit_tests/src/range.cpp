/**
 * @file range.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-12
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "libsfc/range.h"
#include <gtest/gtest.h>
#include "range_test.h"

namespace {
TEST_P(range_test_case, beginIsZeroWhenOnlyEndSupplied)
{
  auto count = end;
  auto iterations = 0;
  for (auto i : ::sfc::range(end)) iterations++;

  EXPECT_EQ(iterations, count);
}

TEST_P(range_test_case, totalItCountIsEndMinusBegin)
{
  auto count = end - begin;
  auto iterations = 0;
  for (auto i : ::sfc::range(begin, end)) iterations++;

  EXPECT_EQ(iterations, count);
}

TEST_P(range_test_case, startsAtBegin)
{
  auto start = begin;
  auto it = ::sfc::range(begin, end);

  EXPECT_EQ(*it, start);
}

TEST_P(range_test_case, stopsAtEndExcluding)
{
  auto step = 1;
  auto last_expected = end - step;
  auto last_actual = 0;
  for (auto i : ::sfc::range(begin, end)) last_actual = i;

  EXPECT_EQ(last_actual, last_expected);
}

TEST_P(range_test_case, incrementsByOneForEachIteration)
{
  auto expected_step = 1;
  auto prev_it = begin - expected_step;
  for (auto i : ::sfc::range(begin, end)) {
    EXPECT_EQ(i - prev_it, expected_step);
    prev_it = i;
  }
}

INSTANTIATE_TEST_CASE_P(sfc_range, range_test_case,
                        ::testing::Combine(::testing::Range<sfc::size_t>(0, 5),
                                           ::testing::Range<sfc::size_t>(10,
                                                                         15)));

};  // namespace
