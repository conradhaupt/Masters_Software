#include "libsfc/pow.h"
#include <gtest/gtest.h>
#include <cmath>
#include "pow_test.h"

namespace {
TEST(isMultipleOf, trueIfIsMultiple) {
  for (auto base : {2, 3, 4, 5}) {
    for (auto multiple : {6, 7, 8, 9}) {
      auto val = base * multiple;
      EXPECT_TRUE(sfc::isMultipleOf(val, base));
      EXPECT_TRUE(sfc::isMultipleOf(val, multiple));
    }
  }
}

TEST(isMultipleOf, falseIfNotMultiple) {
  for (auto base : {2, 3, 4, 5}) {
    for (auto multiple : {6, 7, 8, 9}) {
      auto val = base * multiple;
      EXPECT_FALSE(sfc::isMultipleOf(val - 1, base));
      EXPECT_FALSE(sfc::isMultipleOf(val - 1, multiple));
    }
  }
}

TEST_P(pow_test_case, matchesCMathPow) {
  auto ans_sfc = sfc::pow(base, exp);
  auto ans_std = sfc::size_t(std::pow(double(base), double(exp)));
  EXPECT_EQ(ans_sfc, ans_std);
}

INSTANTIATE_TEST_CASE_P(
    sfc_pow, pow_test_case,
    ::testing::Combine(::testing::Range<sfc::size_t>(0, 64, 4),
                       ::testing::Range<sfc::size_t>(1, 4)));
};  // namespace
