/**
 * @file bwt.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-09-18
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef LIBSFCC_BWT_H
#define LIBSFCC_BWT_H

#include <algorithm>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

namespace sfcc {

namespace bwt {
template <typename T>
struct suffix
{
 private:
  const T* begin;
  const T* end;

 public:
  suffix(const T* begin, const T* end) : begin{begin}, end{end} {}
  bool compare(const T* lhs, const T* rhs)
  {
    // if (lhs > end || lhs < begin || rhs > end || rhs < begin)
    //   throw std::runtime_error("pointers must be between begin and end");
    auto lhs_length = end - lhs;
    auto rhs_length = end - rhs;
    auto result =
        std::memcmp(lhs, rhs, std::min(lhs_length, rhs_length) * sizeof(T));
    if (result < 0)
      return true;
    else if (result > 0)
      return false;
    else
      return rhs_length > lhs_length;
  }
};
}  // namespace bwt

template <typename T>
std::tuple< ::std::unique_ptr<std::uint8_t[]>, std::uint32_t, std::uint32_t>
easyBWT(T* data, unsigned long long length)
{
  // Check if the block is all the same
  auto [min, max] = std::minmax_element(data, data + length);
  std::cout << "BWT data with min=" << std::int16_t(*min)
            << " and max=" << std::int16_t(*max) << std::endl;
  if (*min == *max) {
    // Block is all the same value
    auto bwt_output =
        std::make_unique<std::uint8_t[]>((length + 1) * sizeof(T));
    std::generate((T*)bwt_output.get(), (T*)bwt_output.get() + length + 1,
                  [&min]() { return *min; });

    return {std::move(bwt_output), std::uint32_t{}, std::uint32_t{}};
  }

  bwt::suffix<T> suff(data, data + length);
  auto data_set = std::make_unique<T*[]>(length + 1);
  auto start = data;
  std::generate(data_set.get(), data_set.get() + length + 1,
                [&start]() { return start++; });
  std::sort(data_set.get(), data_set.get() + length + 1,
            std::bind(&bwt::suffix<T>::compare, suff, std::placeholders::_1,
                      std::placeholders::_2));
  auto bwt_output = std::make_unique<T[]>(length + 1);
  auto index = 0ULL;
  std::uint32_t first;
  std::uint32_t last;
  auto it = data_set.get();
  for (auto index = 0ULL; index < length + 1; index++, it++) {
    //  If the output is the end-of-line
    if (*it == data) {
      last = index;
      if (index == 0ULL)
        // Make the current output value the next one
        bwt_output[index] = *(*(it + 1) - 1);
      else
        bwt_output[index] = bwt_output[index - 1];
      continue;

    } else {
      if (*it == (data + 1)) first = index;
      // if the value is neither the beginning or the end
      bwt_output[index] = *(*it - 1);
    }
  }
  auto output = std::make_unique<std::uint8_t[]>((length + 1) * sizeof(T));
  std::move((std::uint8_t*)bwt_output.get(),
            ((std::uint8_t*)bwt_output.get()) + ((length + 1) * sizeof(T)),
            output.get());
  std::cout << "BWT_OUTPUT Size: " << (length + 1) * sizeof(T) << std::endl;
  return {std::move(output), first, last};
}

}  // namespace sfcc

#endif