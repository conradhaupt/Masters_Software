#ifndef TEST_COMPRESSOR_DATA_H
#define TEST_COMPRESSOR_DATA_H

#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include "sfcc.h"

#ifndef TEST_COMPRESSOR_DATA_FILENAME
#error TEST_COMPRESSOR_DATA_FILENAME required for Compressor tests but it is not defined
#endif

class CompressorDataTestFicture
    : public ::testing::TestWithParam<std::tuple<std::string, int>>
{
 protected:
  sfc::sfcc_file file;
  int expectedSize;

 public:
  CompressorDataTestFicture()
      : file{std::get<0>(GetParam())}, expectedSize{std::get<1>(GetParam())}
  {}
};

#endif