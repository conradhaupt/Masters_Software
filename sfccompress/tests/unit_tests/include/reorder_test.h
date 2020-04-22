/**
 * @file reorder_test.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFCC_REORDER_TEST_H
#define SFCC_REORDER_TEST_H
#include <gtest/gtest.h>
#include <tuple>
#include "main.h"
#include "reorder.h"

template <typename FROM, typename TO>
struct SFCurveTypes
{
  using from_t = FROM;
  using to_t = TO;
};
template <class Types>
class reorder_test_case : public ::testing::Test
{
 public:
  using from_t = typename Types::from_t;
  using to_t = typename Types::to_t;
};

class reorder_param_test_case
    : public ::testing::TestWithParam<
          std::tuple<sfc::main::sfcs::types, sfc::main::sfcs::types>>
{
 public:
  sfc::main::sfcs::types from_type = std::get<0>(GetParam());
  sfc::main::sfcs::types to_type = std::get<1>(GetParam());
};
#endif