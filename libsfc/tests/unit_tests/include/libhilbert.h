/**
 * @file libhilbert.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-21
 *
 * @copyright Copyright (c) 2020
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
