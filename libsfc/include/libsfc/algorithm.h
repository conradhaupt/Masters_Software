/**
 * @file algorithm.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2018-10-15
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFC_ALGORITHM_H
#define SFC_ALGORITHM_H

#include <iterator>
namespace sfc {

/**
 * @brief Call a function for each combination of elements in a sequence.
 *
 * @param first the forward iterator to start from
 * @param last the forward iterator to end at
 * @param binary_func the binary function to call for each combination
 * @return BinaryFunction binary_func
 *
 * Applies the function object binary_func to each 2-element combination in the
 * range [first,last).  binary_func must not modify the order of the sequence.
 * If binary_func has a return value it is ignored.
 *
 * binary_func must have the following signature.
 * @code{.cpp}
 * void binary_func(const Type& t1, const Type& t2);
 * @endcode
 */
template <class ForwardIt, class BinaryFunction>
constexpr BinaryFunction for_each_combination(ForwardIt first, ForwardIt last,
                                              BinaryFunction binary_func)
{
  for (; first != last; ++first) {
    auto first_2 = first;
    std::advance(first_2, 1);
    for (; first_2 != last; ++first_2) {
      binary_func(*first, *first_2);
    }
  }
  return binary_func;
}

/**
 * @brief Call a function for each sub-range combination in a sequence.
 *
 * @param first the forward iterator to start from
 * @param last the forward iterator to end at
 * @param binary_func the binary function to call for each sub-range combination
 * @return BinaryFunction binary_func
 *
 * Applies the function object binary_func to each sub-range combination in the
 * range [first,last).  binary_func must not modify the order of the sequence.
 * If binary_func has a return value it is ignored.
 *
 * binary_func must have the following signature.
 * @code{.cpp}
 * void binary_func(ForwardIt begin, ForwardIt end);
 * @endcode
 */
template <class ForwardIt, class BinaryFunction>
constexpr BinaryFunction for_each_range_combination(ForwardIt first,
                                                    ForwardIt last,
                                                    BinaryFunction binary_func)
{
  for (; first != last; ++first) {
    auto first_2 = first;
    std::advance(first_2, 1);
    for (; first_2 != last; ++first_2) {
      binary_func(first, first_2);
    }
  }
  return binary_func;
}
};  // namespace sfc
#endif