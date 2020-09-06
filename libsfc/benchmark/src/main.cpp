#include <benchmark/benchmark.h>
#include <libsfc/gray.h>
#include <libsfc/hilbert.h>
#include <libsfc/morton.h>
#include <libsfc/raster.h>
#include <libsfc/sfcurve.h>
#include <cmath>
#include <iostream>
#include <string>

// process forward and reverse mappings for all SFCs

static void BM_Raster_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);
  auto raster = sfc::raster<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::raster<2>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(raster.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Raster_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Morton_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);
  auto morton = sfc::morton<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::morton<2>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(morton.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Morton_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Gray_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);
  auto gray = sfc::gray<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::gray<2>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(gray.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Gray_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Hilbert_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);
  auto hilbert = sfc::hilbert(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<2>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Hilbert_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Raster_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto raster = sfc::raster<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::raster<2>::coords_type coords = {rand() % sidelength,
                                          rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(raster.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Raster_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Morton_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto morton = sfc::morton<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::morton<2>::coords_type coords = {rand() % sidelength,
                                          rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(morton.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Morton_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Gray_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto gray = sfc::gray<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::gray<2>::coords_type coords = {rand() % sidelength,
                                        rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(gray.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Gray_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Hilbert_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto hilbert = sfc::hilbert(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<2>::coords_type coords = {rand() % sidelength,
                                           rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Hilbert_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

BENCHMARK_MAIN();
