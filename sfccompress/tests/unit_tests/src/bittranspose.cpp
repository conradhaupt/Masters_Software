/**
 * @file bittranspose.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-11-05
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "bittranspose.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <ios>

TEST(bittranspose, transposesSingleBlock_64)
{
  // Create input values
  const auto inputLength = sizeof(std::uint64_t) * CHAR_BIT;
  std::uint64_t values[] = {
      0x1eb993150f58c450, 0x941193e6d7c96664, 0x9bae135bebe5dcd7,
      0x43985f101ffb7bdc, 0xd520ea9ecc3b19ef, 0x1c06e912dae84ba9,
      0x321437f71a1b944c, 0xc17d14692a2b527c, 0x3687fef2e07f347f,
      0x8a5a6523eabcfe94, 0x459599bc0525bf4e, 0x248afbf67a8669c2,
      0x787f6818034bb92f, 0x9cfd7ec6698804ef, 0x04f42cd19d29714b,
      0x563db9127f0ca0f7, 0x1bbfb6fc80a56987, 0xee7e64700c070634,
      0xb3335900817d9450, 0x7ec65f5b2218ad02, 0x04ad256f8d12126d,
      0x2ba244ebcedd7f38, 0x9b08c5977d8c795c, 0x66540b5d8dfd5831,
      0x3a8f1f16a46c463e, 0xe45712fc6c387648, 0x0cd298131ac8812e,
      0xf331bde6817d9c59, 0x94bff958df8d1bfa, 0x0caa3274a779e4ea,
      0xdf82102aafb6e4ac, 0x1145de97b93fdedc, 0xc95b9794c4e2eac3,
      0x3442d261fb6a7db9, 0x80f560ba0361f609, 0x958a2644f98022d4,
      0x88609b81ee521611, 0x9a037eed9bafd9c9, 0x5b0d2a5520b7b42e,
      0x227ed39387ce87f7, 0x13b471d6b2014824, 0x6f1c4535c380eaef,
      0xc22dc426a094a35d, 0x6f35b4c2a84ca3bc, 0x91ca30540b7fc20f,
      0x16b0d02c11d111c7, 0x0ce3e75996353dd2, 0x4bdb43f73aee58ba,
      0x18571a0a8e65d325, 0x3139b47b46b7ab86, 0x28f296080a6a73b8,
      0x490dfe0eb0edf0e0, 0x9e66fa921c1121e0, 0xb6235b76a12c0a12,
      0xab7ae4ba59575117, 0xe69809e7cb3bd2fe, 0xfb0ac67760064fad,
      0x58f2e7e54b0a8201, 0x4679e69818ea5e89, 0x5746fa3062fded35,
      0x84ab09ea46b5d71f, 0xbc3ee4bc868c2127, 0xd742dcfc907fb402,
      0x3d60455dafc132b3};

  std::uint64_t expected[] = {
      0xbf4136b70891f134, 0xf0e2f2c13509fd14, 0x39c33fc8c553b7de,
      0x15849e66ff7075f8, 0x98e4cc9a99e6838d, 0xa99d8bc275b2b1b2,
      0x9866a9ba54eddf,   0x859eeaabf001aa3c, 0x39576ca214695c38,
      0x97a71e9d933206aa, 0x5d0040d6eb2e2747, 0xd22c32398e91e3c,
      0xd4cde0769af457dc, 0xe81e4e4f6269df0a, 0x1dcd9327e3e14aaf,
      0x5a8b1ee5ec0c9645, 0xd8db7164b8a755de, 0x47c690f3c01219d8,
      0x796bdce0d9e78f04, 0x6eac98a2bfecf3fb, 0x58d27450eabc0359,
      0x5cafd067eb8547bc, 0xcc4db8972da4112f, 0xbc0aa6e954e12a2e,
      0x82e032aef8d4f40f, 0xba87d3b674289aef, 0xb013409173f2c41b,
      0x86d5983ad6f2eafd, 0x4458e12a9440c86a, 0x89288d5ae348ab84,
      0x1bc2021b1260ab36, 0xe0a94fbbf9b54336, 0x8382c2f284f842c5,
      0x11fb8d84cd78ab76, 0xe3a8b769abd12c53, 0xf44f602452b91494,
      0xed72d3c5b7cbdd7d, 0x7be2a6266a330fc2, 0xd3a2d96a3abb69c6,
      0x76d089b58a616d52, 0x92a0c39119dc8e6f, 0xf3dc0fba389295f,
      0xe74e4e29897b63c8, 0x58b900709d8cfd38, 0x483f39b3ff0dadcf,
      0x2e5a596c3813fb70, 0xef78e7a6906e3b38, 0x6f5e6c939c418d33,
      0x142bcc659b15b583, 0x7b75d0ab772f1b24, 0x28190fc4939bf5e4,
      0x35ca96c93153ba8d, 0x26c7ab851e87f6cb, 0xb6766d943837f095,
      0xce55d097868a7280, 0x1284f10c75396d0d, 0xc94a9b49c865049c,
      0x4df0afe049ef834d, 0xfcb06a0a769aed33, 0xa35dca71656b3225,
      0xeb33316ad94db177, 0xa1e60a820bae1940, 0x4f888e414a8a9498,
      0x71f0143d5a462296};

  // Declare output memory
  std::uint64_t** output = new std::uint64_t*[inputLength]();
  for (auto i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; i++) {
    output[i] = new std::uint64_t();
  }

  sfcc::bittranspose(values, output);

  for (auto v = 0; v < inputLength; v++) {
    EXPECT_EQ(output[v][0], expected[v]);
  }
  for (auto i = 0; i < inputLength; i++) delete output[i];
  delete[] output;
}

TEST(bittranspose, tranposeIsReversable_64)
{
  const auto inputLength = sizeof(std::uint64_t) * CHAR_BIT;
  std::uint64_t* values = new std::uint64_t[inputLength]();
  std::uint64_t val = 0;
  std::generate(values, values + inputLength, [&val]() { return val++; });

  // Declare output memory
  std::uint64_t** output = new std::uint64_t*[inputLength];
  for (auto i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; i++) {
    output[i] = new std::uint64_t();
  }

  // Forward operation
  sfcc::bittranspose(values, output);

  // Reverse operation
  std::uint64_t* reversed_values = new std::uint64_t[inputLength]();
  sfcc::reverse_bittranspose(output, reversed_values);

  // Compare reversed and original
  for (auto i = 0; i < inputLength; i++) {
    EXPECT_EQ(values[i], reversed_values[i]);
  }

  delete[] values;
  for (auto i = 0; i < inputLength; i++) delete output[i];
  delete[] output;
  delete[] reversed_values;
}

TEST(bittranspose, reverseTransposeIsCorrect_64)
{
  const auto inputLength = sizeof(std::uint64_t) * CHAR_BIT;
  std::uint64_t* values = new std::uint64_t[inputLength]();
  std::uint64_t val = 0;
  std::generate(values, values + inputLength, [&val]() { return val++; });

  // Declare output memory
  std::uint64_t** output = new std::uint64_t*[inputLength];
  for (auto i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; i++) {
    output[i] = new std::uint64_t();
  }

  // Forward operation
  sfcc::bittranspose(values, output);

  // Reverse operation
  std::uint64_t* reversed_values = new std::uint64_t[inputLength]();
  sfcc::reverse_bittranspose(output, reversed_values);

  // Compare reversed and original
  for (auto i = 0; i < inputLength; i++) {
    EXPECT_EQ(values[i], reversed_values[i]);
  }
  delete[] reversed_values;
  for (auto i = 0; i < inputLength; i++) {
    delete output[i];
  }
  delete[] output;
  delete[] values;
}

TEST(BitTransposer, tranposeIsReversable_64)
{
  const auto length = 256;
  std::uint64_t values[] = {
      0x1eb993150f58c450, 0x941193e6d7c96664, 0x9bae135bebe5dcd7,
      0x43985f101ffb7bdc, 0xd520ea9ecc3b19ef, 0x1c06e912dae84ba9,
      0x321437f71a1b944c, 0xc17d14692a2b527c, 0x3687fef2e07f347f,
      0x8a5a6523eabcfe94, 0x459599bc0525bf4e, 0x248afbf67a8669c2,
      0x787f6818034bb92f, 0x9cfd7ec6698804ef, 0x04f42cd19d29714b,
      0x563db9127f0ca0f7, 0x1bbfb6fc80a56987, 0xee7e64700c070634,
      0xb3335900817d9450, 0x7ec65f5b2218ad02, 0x04ad256f8d12126d,
      0x2ba244ebcedd7f38, 0x9b08c5977d8c795c, 0x66540b5d8dfd5831,
      0x3a8f1f16a46c463e, 0xe45712fc6c387648, 0x0cd298131ac8812e,
      0xf331bde6817d9c59, 0x94bff958df8d1bfa, 0x0caa3274a779e4ea,
      0xdf82102aafb6e4ac, 0x1145de97b93fdedc, 0xc95b9794c4e2eac3,
      0x3442d261fb6a7db9, 0x80f560ba0361f609, 0x958a2644f98022d4,
      0x88609b81ee521611, 0x9a037eed9bafd9c9, 0x5b0d2a5520b7b42e,
      0x227ed39387ce87f7, 0x13b471d6b2014824, 0x6f1c4535c380eaef,
      0xc22dc426a094a35d, 0x6f35b4c2a84ca3bc, 0x91ca30540b7fc20f,
      0x16b0d02c11d111c7, 0x0ce3e75996353dd2, 0x4bdb43f73aee58ba,
      0x18571a0a8e65d325, 0x3139b47b46b7ab86, 0x28f296080a6a73b8,
      0x490dfe0eb0edf0e0, 0x9e66fa921c1121e0, 0xb6235b76a12c0a12,
      0xab7ae4ba59575117, 0xe69809e7cb3bd2fe, 0xfb0ac67760064fad,
      0x58f2e7e54b0a8201, 0x4679e69818ea5e89, 0x5746fa3062fded35,
      0x84ab09ea46b5d71f, 0xbc3ee4bc868c2127, 0xd742dcfc907fb402,
      0x3d60455dafc132b3, 0x1eb993150f58c450, 0x941193e6d7c96664,
      0x9bae135bebe5dcd7, 0x43985f101ffb7bdc, 0xd520ea9ecc3b19ef,
      0x1c06e912dae84ba9, 0x321437f71a1b944c, 0xc17d14692a2b527c,
      0x3687fef2e07f347f, 0x8a5a6523eabcfe94, 0x459599bc0525bf4e,
      0x248afbf67a8669c2, 0x787f6818034bb92f, 0x9cfd7ec6698804ef,
      0x04f42cd19d29714b, 0x563db9127f0ca0f7, 0x1bbfb6fc80a56987,
      0xee7e64700c070634, 0xb3335900817d9450, 0x7ec65f5b2218ad02,
      0x04ad256f8d12126d, 0x2ba244ebcedd7f38, 0x9b08c5977d8c795c,
      0x66540b5d8dfd5831, 0x3a8f1f16a46c463e, 0xe45712fc6c387648,
      0x0cd298131ac8812e, 0xf331bde6817d9c59, 0x94bff958df8d1bfa,
      0x0caa3274a779e4ea, 0xdf82102aafb6e4ac, 0x1145de97b93fdedc,
      0xc95b9794c4e2eac3, 0x3442d261fb6a7db9, 0x80f560ba0361f609,
      0x958a2644f98022d4, 0x88609b81ee521611, 0x9a037eed9bafd9c9,
      0x5b0d2a5520b7b42e, 0x227ed39387ce87f7, 0x13b471d6b2014824,
      0x6f1c4535c380eaef, 0xc22dc426a094a35d, 0x6f35b4c2a84ca3bc,
      0x91ca30540b7fc20f, 0x16b0d02c11d111c7, 0x0ce3e75996353dd2,
      0x4bdb43f73aee58ba, 0x18571a0a8e65d325, 0x3139b47b46b7ab86,
      0x28f296080a6a73b8, 0x490dfe0eb0edf0e0, 0x9e66fa921c1121e0,
      0xb6235b76a12c0a12, 0xab7ae4ba59575117, 0xe69809e7cb3bd2fe,
      0xfb0ac67760064fad, 0x58f2e7e54b0a8201, 0x4679e69818ea5e89,
      0x5746fa3062fded35, 0x84ab09ea46b5d71f, 0xbc3ee4bc868c2127,
      0xd742dcfc907fb402, 0x3d60455dafc132b3, 0x1eb993150f58c450,
      0x941193e6d7c96664, 0x9bae135bebe5dcd7, 0x43985f101ffb7bdc,
      0xd520ea9ecc3b19ef, 0x1c06e912dae84ba9, 0x321437f71a1b944c,
      0xc17d14692a2b527c, 0x3687fef2e07f347f, 0x8a5a6523eabcfe94,
      0x459599bc0525bf4e, 0x248afbf67a8669c2, 0x787f6818034bb92f,
      0x9cfd7ec6698804ef, 0x04f42cd19d29714b, 0x563db9127f0ca0f7,
      0x1bbfb6fc80a56987, 0xee7e64700c070634, 0xb3335900817d9450,
      0x7ec65f5b2218ad02, 0x04ad256f8d12126d, 0x2ba244ebcedd7f38,
      0x9b08c5977d8c795c, 0x66540b5d8dfd5831, 0x3a8f1f16a46c463e,
      0xe45712fc6c387648, 0x0cd298131ac8812e, 0xf331bde6817d9c59,
      0x94bff958df8d1bfa, 0x0caa3274a779e4ea, 0xdf82102aafb6e4ac,
      0x1145de97b93fdedc, 0xc95b9794c4e2eac3, 0x3442d261fb6a7db9,
      0x80f560ba0361f609, 0x958a2644f98022d4, 0x88609b81ee521611,
      0x9a037eed9bafd9c9, 0x5b0d2a5520b7b42e, 0x227ed39387ce87f7,
      0x13b471d6b2014824, 0x6f1c4535c380eaef, 0xc22dc426a094a35d,
      0x6f35b4c2a84ca3bc, 0x91ca30540b7fc20f, 0x16b0d02c11d111c7,
      0x0ce3e75996353dd2, 0x4bdb43f73aee58ba, 0x18571a0a8e65d325,
      0x3139b47b46b7ab86, 0x28f296080a6a73b8, 0x490dfe0eb0edf0e0,
      0x9e66fa921c1121e0, 0xb6235b76a12c0a12, 0xab7ae4ba59575117,
      0xe69809e7cb3bd2fe, 0xfb0ac67760064fad, 0x58f2e7e54b0a8201,
      0x4679e69818ea5e89, 0x5746fa3062fded35, 0x84ab09ea46b5d71f,
      0xbc3ee4bc868c2127, 0xd742dcfc907fb402, 0x3d60455dafc132b3,
      0x1eb993150f58c450, 0x941193e6d7c96664, 0x9bae135bebe5dcd7,
      0x43985f101ffb7bdc, 0xd520ea9ecc3b19ef, 0x1c06e912dae84ba9,
      0x321437f71a1b944c, 0xc17d14692a2b527c, 0x3687fef2e07f347f,
      0x8a5a6523eabcfe94, 0x459599bc0525bf4e, 0x248afbf67a8669c2,
      0x787f6818034bb92f, 0x9cfd7ec6698804ef, 0x04f42cd19d29714b,
      0x563db9127f0ca0f7, 0x1bbfb6fc80a56987, 0xee7e64700c070634,
      0xb3335900817d9450, 0x7ec65f5b2218ad02, 0x04ad256f8d12126d,
      0x2ba244ebcedd7f38, 0x9b08c5977d8c795c, 0x66540b5d8dfd5831,
      0x3a8f1f16a46c463e, 0xe45712fc6c387648, 0x0cd298131ac8812e,
      0xf331bde6817d9c59, 0x94bff958df8d1bfa, 0x0caa3274a779e4ea,
      0xdf82102aafb6e4ac, 0x1145de97b93fdedc, 0xc95b9794c4e2eac3,
      0x3442d261fb6a7db9, 0x80f560ba0361f609, 0x958a2644f98022d4,
      0x88609b81ee521611, 0x9a037eed9bafd9c9, 0x5b0d2a5520b7b42e,
      0x227ed39387ce87f7, 0x13b471d6b2014824, 0x6f1c4535c380eaef,
      0xc22dc426a094a35d, 0x6f35b4c2a84ca3bc, 0x91ca30540b7fc20f,
      0x16b0d02c11d111c7, 0x0ce3e75996353dd2, 0x4bdb43f73aee58ba,
      0x18571a0a8e65d325, 0x3139b47b46b7ab86, 0x28f296080a6a73b8,
      0x490dfe0eb0edf0e0, 0x9e66fa921c1121e0, 0xb6235b76a12c0a12,
      0xab7ae4ba59575117, 0xe69809e7cb3bd2fe, 0xfb0ac67760064fad,
      0x58f2e7e54b0a8201, 0x4679e69818ea5e89, 0x5746fa3062fded35,
      0x84ab09ea46b5d71f, 0xbc3ee4bc868c2127, 0xd742dcfc907fb402,
      0x3d60455dafc132b3};

  std::uint64_t* transformed_values = new std::uint64_t[256];
  std::copy(values, values + length, transformed_values);

  // Forward operation
  auto bitTransp = sfcc::BitTransposer();
  bitTransp.transpose_t<std::uint64_t>(transformed_values, length);

  // Retrieve output values

  // Reverse operation
  std::uint64_t* reversed_values = new std::uint64_t[256]();
  std::copy(transformed_values, transformed_values + length, reversed_values);
  bitTransp.reverse_transpose_t<std::uint64_t>(reversed_values, length);

  // Compare reversed and original
  for (auto i = 0; i < length; i++) {
    EXPECT_EQ(values[i], reversed_values[i]);
  }

  delete[] transformed_values;
  delete[] reversed_values;
}