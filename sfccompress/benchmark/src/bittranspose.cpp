#include <benchmark/benchmark.h>
#include <bittranspose.h>
#include <bittranspose_utils.h>
#include <memory>

// Forward bittranspose

template <typename T, typename U>
static void BM_Bittranspose_Forward(benchmark::State& state)
{
  static_assert(sizeof(T) == sizeof(U));
  // state.PauseTiming();
  const auto sidelength = state.range(0);
  const auto ndims = state.range(1);
  const auto nelems = std::pow(sidelength, ndims);
  auto data = CreateRandomNDArray<T>(sidelength, ndims);
  sfcc::BitTransposer bittranposer;
  // state.ResumeTiming();
  for (auto _ : state) {
    bittranposer.transpose_t<U>((U*)data.get(), nelems);
  }
  state.SetBytesProcessed(state.iterations() * nelems * sizeof(T));
}

BENCHMARK_TEMPLATE(BM_Bittranspose_Forward, std::int16_t, std::int16_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 12}, {2, 2}});
BENCHMARK_TEMPLATE(BM_Bittranspose_Forward, std::int16_t, std::int16_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 10}, {3, 3}});

BENCHMARK_TEMPLATE(BM_Bittranspose_Forward, float, std::int32_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 11}, {2, 2}});
BENCHMARK_TEMPLATE(BM_Bittranspose_Forward, float, std::int32_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 9}, {3, 3}});

// Reverse Bittranspose

template <typename T, typename U>
static void BM_Bittranspose_Reverse(benchmark::State& state)
{
  static_assert(sizeof(T) == sizeof(U));
  // state.PauseTiming();
  const auto sidelength = state.range(0);
  const auto ndims = state.range(1);
  const auto nelems = std::pow(sidelength, ndims);
  auto data = CreateRandomNDArray<T>(sidelength, ndims);
  sfcc::BitTransposer bittranposer;
  // state.ResumeTiming();
  for (auto _ : state) {
    bittranposer.reverse_transpose_t<U>((U*)data.get(), nelems);
  }
  state.SetBytesProcessed(state.iterations() * nelems * sizeof(T));
}

BENCHMARK_TEMPLATE(BM_Bittranspose_Reverse, std::int16_t, std::int16_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 12}, {2, 2}});
BENCHMARK_TEMPLATE(BM_Bittranspose_Reverse, std::int16_t, std::int16_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 10}, {3, 3}});

BENCHMARK_TEMPLATE(BM_Bittranspose_Reverse, float, std::int32_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 11}, {2, 2}});
BENCHMARK_TEMPLATE(BM_Bittranspose_Reverse, float, std::int32_t)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 4, 1 << 9}, {3, 3}});