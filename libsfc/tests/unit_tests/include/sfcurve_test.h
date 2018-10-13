/**
 * @file sfcurve_test.h
 * @author Conrad Haupt
 * @date 2018-07-01
 */

#ifndef SFC_SFCURVE_TEST_H
#define SFC_SFCURVE_TEST_H
#include <gtest/gtest.h>
#include <tuple>
#include "libsfc/sfcurve.h"

class sfcurve_test_case
    : public ::testing::TestWithParam<
          std::tuple<double, double, typename sfc::sfcurve<2>::size_type>>
{
 public:
  using index_type = typename sfc::sfcurve<2>::index_type;
  typename sfc::sfcurve<2>::size_type dimLength{std::get<2>(GetParam())};
  typename sfc::sfcurve<2>::coords_type coordinates{
      index_type(dimLength * std::get<0>(GetParam())),
      index_type(dimLength* std::get<1>(GetParam()))};
};
#endif
