/**
 * @file interleave_test.h
 * @author Conrad Haupt
 * @date 2018-07-03
 */

#ifndef SFC_INTERLEAVE_TEST_H
#define SFC_INTERLEAVE_TEST_H
#include <gtest/gtest.h>
#include <array>
#include "libsfc/interleave.h"
#include "libsfc/sfcdef.h"

class interleave_test_case
    : public ::testing::TestWithParam<std::tuple<uint32_t, uint32_t>>
{
 public:
  typedef uint32_t _TpDist;
  typedef uint32_t _TpCoord;
  typedef std::array<_TpCoord, 2> _TpCoords;
  _TpCoords coords = {std::get<0>(GetParam()), std::get<1>(GetParam())};
};
#endif
