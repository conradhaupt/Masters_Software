/**
 * @file range.h
 * @author Conrad Haupt
 * @brief
 * @version 0.1
 * @date 2018-10-04
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef SFC_RANGE_TEST_H
#define SFC_RANGE_TEST_H
#include <gtest/gtest.h>
#include "libsfc/range.h"

class range_test_case
    : public ::testing::TestWithParam<std::tuple<sfc::size_t, sfc::size_t>>
{
 public:
  sfc::size_t begin = std::get<0>(GetParam());
  sfc::size_t end = std::get<1>(GetParam());
};
#endif
