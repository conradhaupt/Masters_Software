#include <benchmark/benchmark.h>
#include <libsfc/gray.h>
#include <libsfc/hilbert.h>
#include <libsfc/morton.h>
#include <libsfc/raster.h>
#include <libsfc/sfcurve.h>
#include <cmath>
#include <iostream>
#include <string>

// process forward and reverse mappings for all SFCs in two-dimensions

static void BM_Raster_2_Forward(benchmark::State& state)
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

BENCHMARK(BM_Raster_2_Forward)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Morton_2_Forward(benchmark::State& state)
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

BENCHMARK(BM_Morton_2_Forward)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Gray_2_Forward(benchmark::State& state)
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

BENCHMARK(BM_Gray_2_Forward)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Hilbert_2_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);
  auto hilbert = sfc::hilbert<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<2>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Hilbert_2_Forward)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Raster_2_Reverse(benchmark::State& state)
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

BENCHMARK(BM_Raster_2_Reverse)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Morton_2_Reverse(benchmark::State& state)
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

BENCHMARK(BM_Morton_2_Reverse)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Gray_2_Reverse(benchmark::State& state)
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

BENCHMARK(BM_Gray_2_Reverse)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

static void BM_Hilbert_2_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto hilbert = sfc::hilbert<2>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<2>::coords_type coords = {rand() % sidelength,
                                           rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Hilbert_2_Reverse)->RangeMultiplier(2)->Range(1 << 9, 1 << 12);

// Process forward and reverse mappings for all SFCs in three-dimensions
static void BM_Raster_3_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);
  auto raster = sfc::raster<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::raster<3>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(raster.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Raster_3_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Morton_3_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);
  auto morton = sfc::morton<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::morton<3>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(morton.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Morton_3_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Gray_3_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);
  auto gray = sfc::gray<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::gray<3>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(gray.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Gray_3_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Hilbert_3_Forward(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);
  auto hilbert = sfc::hilbert<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<3>::dist_type dist = rand() % max_dist;
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.distToCoords(dist));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Hilbert_3_Forward)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Raster_3_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto raster = sfc::raster<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::raster<3>::coords_type coords = {
        rand() % sidelength, rand() % sidelength, rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(raster.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Raster_3_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Morton_3_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto morton = sfc::morton<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::morton<3>::coords_type coords = {
        rand() % sidelength, rand() % sidelength, rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(morton.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Morton_3_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Gray_3_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto gray = sfc::gray<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::gray<3>::coords_type coords = {
        rand() % sidelength, rand() % sidelength, rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(gray.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Gray_3_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);

static void BM_Hilbert_3_Reverse(benchmark::State& state)
{
  const unsigned long long sidelength = state.range(0);
  auto hilbert = sfc::hilbert<3>(sidelength);
  for (auto _ : state) {
    state.PauseTiming();
    sfc::hilbert<3>::coords_type coords = {
        rand() % sidelength, rand() % sidelength, rand() % sidelength};
    state.ResumeTiming();
    benchmark::DoNotOptimize(hilbert.coordsToDist(coords));
  }
  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Hilbert_3_Reverse)->RangeMultiplier(2)->Range(1 << 4, 1 << 8);