/**
 * @file mtf.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef LIBSFCC_MTF_H
#define LIBSFCC_MTF_H
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <memory>

namespace sfcc {

// Returns the MTF encoded sequence and the min and max elements
template <typename T, typename TIndex>
std::tuple<::std::unique_ptr<TIndex[]>, T, T> easyMTFAnyuType(
    const T* data, unsigned long long length)
{
  auto [min, max] = ::std::minmax_element(data, data + length);
  const auto min_elem = *min;
  const auto max_elem = *max;
  std::cout << "MTF: " << std::int64_t(min_elem) << " to "
            << std::int64_t(max_elem) << std::endl;
  auto alphabet_size = 0ULL;
  std::list<T> alphabet;
  auto prev = min_elem;
  for (auto minimum = *min; minimum <= *max; minimum++) {
    alphabet.push_back(minimum);
    alphabet_size++;
    if (minimum == *max) break;
  }
  if (1ULL << (sizeof(TIndex) * 8) < alphabet_size)
    throw std::runtime_error(
        "TIndex is not large enough to hold all indices for the given "
        "alphabet");
  std::cout << "Allocating MTF output memory" << std::endl;
  std::unique_ptr<TIndex[]> output = std::make_unique<TIndex[]>(length);
  std::cout << "Finished allocating MTF output memory" << std::endl;
  // Populate output array given input data
  std::transform(data, data + length, output.get(), [&alphabet](T value) {
    TIndex index(-1);
    auto rem = std::find_if(std::begin(alphabet), std::end(alphabet),
                            [&index, &value](auto& a) {
                              index++;
                              return a == value;
                            });
    //  Assert that the the current character is in the alphabet
    assert(rem != std::end(alphabet));
    //  Move the value in the alphabet to the beginning of the
    //  list
    alphabet.erase(rem);
    alphabet.push_front(value);
    return index;
  });
  return {std::move(output), min_elem, max_elem};
}

template <typename T, typename TIndex>
::std::unique_ptr<T[]> easyMTFDecodeAnyuType(const TIndex* data,
                                             unsigned long long length,
                                             const T& min, const T& max)
{
  const auto min_elem = min;
  const auto max_elem = max;
  std::cout << "MTF: " << std::int64_t(min_elem) << " to "
            << std::int64_t(max_elem) << std::endl;
  auto alphabet_size = 0ULL;
  std::list<T> alphabet;
  auto prev = min_elem;
  for (auto minimum = min_elem; minimum <= max_elem; minimum++) {
    alphabet.push_back(minimum);
    alphabet_size++;
    if (minimum == max) break;
  }
  if (1ULL << (sizeof(TIndex) * 8) < alphabet_size)
    throw std::runtime_error(
        "TIndex is not large enough to hold all indices for the given "
        "alphabet");
  std::cout << "Allocating MTF output memory" << std::endl;
  auto output = std::make_unique<T[]>(length);
  std::cout << "Finished allocating MTF output memory" << std::endl;
  // Populate output array given input data
  std::transform(
      data, data + length, output.get(),
      [&alphabet, &min_elem, &max_elem, &alphabet_size](TIndex index) {
        if (index >= alphabet_size) {
          throw std::runtime_error("MTF index is out-of-bounds of alphabet: " +
                                   std::to_string(index) +
                                   ", minmax=" + std::to_string(min_elem) +
                                   ", " + std::to_string(max_elem));
        }
        auto value_it = std::next(std::begin(alphabet), index);
        auto value = *value_it;
        alphabet.erase(value_it);
        alphabet.push_front(value);
        return value;
      });
  return std::move(output);
}

}  // namespace sfcc
#endif