#ifndef BITTRANSPOSE_UTILS_H
#define BITTRANSPOSE_UTILS_H

#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <random>
#include <type_traits>

template <typename T>
std::unique_ptr<T[]> CreateRandomNDArray(const int sidelength, const int ndims);

template <>
std::unique_ptr<std::int16_t[]> CreateRandomNDArray(const int sidelength,
                                                    const int ndims)
{
  const auto nelems = std::pow(sidelength, ndims);
  auto values = std::make_unique<std::int16_t[]>(nelems);

  // Create random number generator for the specific type
  std::minstd_rand gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(INT16_MIN, INT16_MAX);

  for (auto i = 0; i < nelems; i++) {
    values[i] = std::int16_t(dist(gen));
  }

  return std::move(values);
}

template <>
std::unique_ptr<float[]> CreateRandomNDArray(const int sidelength,
                                             const int ndims)
{
  const auto nelems = std::pow(sidelength, ndims);
  auto values = std::make_unique<float[]>(nelems);

  // Create random number generator for the specific type
  std::minstd_rand gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(
      std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

  for (auto i = 0; i < nelems; i++) {
    values[i] = dist(gen);
  }

  return std::move(values);
}

#endif /* BITTRANSPOSE_UTILS_H */
