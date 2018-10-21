/**
 * @file libhilbert.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-10-21
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef SFC_LIBHILBERT_TEST_H
#define SFC_LIBHILBERT_TEST_H
#include <gtest/gtest.h>
#include "libsfc/hilbert_mapping.h"

class libhilbert_test_case : public ::testing::TestWithParam<sfc::size_t>
{
 public:
  sfc::size_t distance = GetParam();
  sfc::size_t dimensionSize = 128ULL;
};
#endif
