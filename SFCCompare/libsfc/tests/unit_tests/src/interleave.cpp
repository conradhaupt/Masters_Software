#include "libsfc/interleave.h"
#include <gtest/gtest.h>
#include <bitset>
#include <string>
#include "interleave_test.h"
#include "libsfc/range.h"

// Wrappers for sfc::max_interleave_dim_length as GTest doesn't like template
// parameters
constexpr uint64_t max_interleave_dim_length_2_u8_u64_t()
{
  return sfc::max_interleave_dim_length<2, uint8_t, uint64_t>();
}
constexpr uint64_t max_interleave_dim_length_2_u16_u64_t()
{
  return sfc::max_interleave_dim_length<2, uint16_t, uint64_t>();
}
constexpr uint64_t max_interleave_dim_length_2_u32_u64_t()
{
  return sfc::max_interleave_dim_length<2, uint32_t, uint64_t>();
}
constexpr uint64_t max_interleave_dim_length_2_u64_u64_t()
{
  return sfc::max_interleave_dim_length<2, uint64_t, uint64_t>();
}
constexpr uint8_t max_interleave_dim_length_2_u16_u8_t()
{
  return sfc::max_interleave_dim_length<2, uint16_t, uint8_t>();
}
constexpr uint8_t max_interleave_dim_length_2_u32_u8_t()
{
  return sfc::max_interleave_dim_length<2, uint32_t, uint8_t>();
}
constexpr uint16_t max_interleave_dim_length_2_u32_u16_t()
{
  return sfc::max_interleave_dim_length<2, uint32_t, uint16_t>();
}
constexpr uint8_t max_interleave_dim_length_2_u64_u8_t()
{
  return sfc::max_interleave_dim_length<2, uint64_t, uint8_t>();
}
constexpr uint16_t max_interleave_dim_length_2_u64_u16_t()
{
  return sfc::max_interleave_dim_length<2, uint64_t, uint16_t>();
}
constexpr uint32_t max_interleave_dim_length_2_u64_u32_t()
{
  return sfc::max_interleave_dim_length<2, uint64_t, uint32_t>();
}
constexpr uint8_t max_interleave_dim_length_2_u8_u8_t()
{
  return sfc::max_interleave_dim_length<2, uint8_t, uint8_t>();
}
constexpr uint16_t max_interleave_dim_length_2_u16_u16_t()
{
  return sfc::max_interleave_dim_length<2, uint16_t, uint16_t>();
}
constexpr uint32_t max_interleave_dim_length_2_u32_u32_t()
{
  return sfc::max_interleave_dim_length<2, uint32_t, uint32_t>();
}

TEST(Interleave, maxDimLengthDividesLargerDistanceType)
{
  EXPECT_EQ(max_interleave_dim_length_2_u8_u64_t(), (1ULL << 4) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u16_u64_t(), (1ULL << 8) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u32_u64_t(), (1ULL << 16) - 1);
}

TEST(Interleave, maxDimLengthUsesSmallerMaxCoordType)
{
  EXPECT_EQ(max_interleave_dim_length_2_u16_u8_t(), (1ULL << 8) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u32_u8_t(), (1ULL << 8) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u32_u16_t(), (1ULL << 16) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u64_u8_t(), (1ULL << 8) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u64_u16_t(), (1ULL << 16) - 1);
  EXPECT_EQ(max_interleave_dim_length_2_u64_u32_t(), (1ULL << 32) - 1);
}

TEST(Interleave, maskCorrectForTwoDim32Bit)
{
  using _Tp = uint32_t;
  auto mask_32 = sfc::interleave_mask<2, _Tp>;
  const _Tp MASK{1};
  const _Tp ONE = MASK;
  const _Tp ZERO{0};
  EXPECT_GT(mask_32, 1);
  EXPECT_EQ(sizeof(_Tp) * CHAR_BIT, 32);
  for (auto i : sfc::range(sizeof(_Tp) * CHAR_BIT / 2)) {
    EXPECT_EQ(mask_32 & MASK, ONE);
    mask_32 >>= 1;
    EXPECT_EQ(mask_32 & MASK, ZERO);
    mask_32 >>= 1;
  }
}

TEST(Interleave, maskCorrectForTwoDim64Bit)
{
  using _Tp = uint64_t;
  auto mask_64 = sfc::interleave_mask<2, _Tp>;
  const _Tp MASK{1};
  const _Tp ONE = MASK;
  const _Tp ZERO{0};
  EXPECT_GT(mask_64, 1);
  EXPECT_EQ(sizeof(_Tp) * CHAR_BIT, 64);
  for (auto i : sfc::range(sizeof(_Tp) * CHAR_BIT / 2)) {
    EXPECT_EQ(mask_64 & MASK, ONE);
    mask_64 >>= 1;
    EXPECT_EQ(mask_64 & MASK, ZERO);
    mask_64 >>= 1;
  }
}

TEST(Interleave, maskCorrectForThreeDim32Bit)
{
  using _Tp = uint32_t;
  auto mask_32 = sfc::interleave_mask<3, _Tp>;
  const _Tp MASK{1};
  const _Tp ONE = MASK;
  const _Tp ZERO{0};
  EXPECT_GT(mask_32, 2);
  EXPECT_EQ(sizeof(_Tp) * CHAR_BIT, 32);
  for (auto i : sfc::range(sizeof(_Tp) * CHAR_BIT / 3)) {
    EXPECT_EQ(mask_32 & MASK, ONE);
    mask_32 >>= 1;
    EXPECT_EQ(mask_32 & MASK, ZERO);
    mask_32 >>= 1;
    EXPECT_EQ(mask_32 & MASK, ZERO);
    mask_32 >>= 1;
  }
}

TEST(Interleave, maskCorrectForThreeDim64Bit)
{
  using _Tp = uint64_t;
  auto mask_64 = sfc::interleave_mask<3, _Tp>;
  const _Tp MASK{1};
  const _Tp ONE = MASK;
  const _Tp ZERO{0};
  EXPECT_GT(mask_64, 2);
  EXPECT_EQ(sizeof(_Tp) * CHAR_BIT, 64);
  for (auto i : sfc::range(sizeof(_Tp) * CHAR_BIT / 3)) {
    EXPECT_EQ(mask_64 & MASK, ONE);
    mask_64 >>= 1;
    EXPECT_EQ(mask_64 & MASK, ZERO);
    mask_64 >>= 1;
    EXPECT_EQ(mask_64 & MASK, ZERO);
    mask_64 >>= 1;
  }
}

TEST_P(interleave_test_case, interleavingIsInvertible)
{
  auto dist = sfc::interleaveBits<2, _TpDist, _TpCoords, _TpCoord>(coords);
  auto resCoords = sfc::uinterleaveBits<2, _TpDist, _TpCoords, _TpCoord>(dist);
  EXPECT_EQ(coords[0], resCoords[0]);
  EXPECT_EQ(coords[1], resCoords[1]);
}

INSTANTIATE_TEST_CASE_P(
    interleave_bits, interleave_test_case,
    ::testing::Combine(::testing::Range<uint32_t>(0, 1 << 15, 4096),
                       ::testing::Range<uint32_t>(0, 1 << 15, 4096)));
