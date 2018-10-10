/**
 * @file pow_test.h
 * @author Conrad Haupt
 * @date 2018-07-03
 */

#ifndef SFC_POW_TEST_H
#define SFC_POW_TEST_H
#include <gtest/gtest.h>
#include <tuple>
#include "libsfc/pow.h"
#include "libsfc/sfcdef.h"

class pow_test_case
    : public ::testing::TestWithParam<std::tuple<sfc::size_t, sfc::size_t>> {
 public:
  sfc::size_t base = std::get<0>(GetParam());
  sfc::size_t exp = std::get<1>(GetParam());
};
#endif
