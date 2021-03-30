#include <benchmark/benchmark.h>
#include <libsfc/gray.h>
#include <libsfc/hilbert.h>
#include <libsfc/morton.h>
#include <libsfc/raster.h>
#include <libsfc/sfcurve.h>
#include <reorder.h>

// Forward reordering

static void BM_Reorder_Raster_2(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Raster_2)->RangeMultiplier(2)->Range(1 << 8, 1 << 12);

static void BM_Reorder_Morton_2(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::MORTON);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Morton_2)->RangeMultiplier(2)->Range(1 << 8, 1 << 12);

static void BM_Reorder_Gray_2(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::GRAY_CODE);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Gray_2)->RangeMultiplier(2)->Range(1 << 8, 1 << 12);

static void BM_Reorder_Hilbert_2(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::HILBERT);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Hilbert_2)->RangeMultiplier(2)->Range(1 << 8, 1 << 12);

// Reverse reordering
static void BM_Reorder_Raster_2_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Raster_2_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 8, 1 << 12);

static void BM_Reorder_Morton_2_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::MORTON,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Morton_2_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 8, 1 << 12);

static void BM_Reorder_Gray_2_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::GRAY_CODE,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Gray_2_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 8, 1 << 12);

static void BM_Reorder_Hilbert_2_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(2, sidelength, sfc::main::sfcs::types::HILBERT,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Hilbert_2_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 8, 1 << 12);

// 
// Forward reordering 3D 
// TODO
static void BM_Reorder_Raster_3(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Raster_3)->RangeMultiplier(2)->Range(1 << 5, 1 << 9);

static void BM_Reorder_Morton_3(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::MORTON);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Morton_3)->RangeMultiplier(2)->Range(1 << 5, 1 << 9);

static void BM_Reorder_Gray_3(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::GRAY_CODE);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Gray_3)->RangeMultiplier(2)->Range(1 << 5, 1 << 9);

static void BM_Reorder_Hilbert_3(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::HILBERT);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Hilbert_3)->RangeMultiplier(2)->Range(1 << 5, 1 << 9);

// Reverse reordering
static void BM_Reorder_Raster_3_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::ROW_MAJOR,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Raster_3_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 5, 1 << 9);

static void BM_Reorder_Morton_3_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::MORTON,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Morton_3_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 5, 1 << 9);

static void BM_Reorder_Gray_3_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::GRAY_CODE,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Gray_3_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 5, 1 << 9);

static void BM_Reorder_Hilbert_3_Reverse(benchmark::State& state)
{
  const auto sidelength = state.range(0);
  const auto max_dist = (sidelength * sidelength * sidelength);

  state.PauseTiming();
  std::uint16_t* values = new std::uint16_t[max_dist];
  benchmark::DoNotOptimize(values);
  if (values == nullptr)
    throw std::runtime_error(
        "Could not allocate values array for reorder benchmark");
  state.ResumeTiming();

  auto reorderer =
      sfcc::make_reorderer(3, sidelength, sfc::main::sfcs::types::HILBERT,
                           sfc::main::sfcs::types::ROW_MAJOR);

  for (auto _ : state) {
    reorderer->reorder_withtemporary(values, max_dist);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * max_dist *
                          sizeof(std::uint16_t));
}

BENCHMARK(BM_Reorder_Hilbert_3_Reverse)
    ->RangeMultiplier(2)
    ->Range(1 << 5, 1 << 9);
