/**
 * @file reorder.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-09-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef LIBSFCC_REORDER_H
#define LIBSFCC_REORDER_H
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include "libsfc/sfcurve.h"
#include "main.h"
namespace sfcc {

class reorderer
{
 private:
  std::unique_ptr<sfc::sfcurve<2>> _from_2;
  std::unique_ptr<sfc::sfcurve<2>> _to_2;

  std::unique_ptr<sfc::sfcurve<3>> _from_3;
  std::unique_ptr<sfc::sfcurve<3>> _to_3;

  const sfc::size_t _NDims;

 public:
  reorderer(std::unique_ptr<sfc::sfcurve<2>>&& from,
            std::unique_ptr<sfc::sfcurve<2>>&& to)
      : _from_2(std::move(from)), _to_2(std::move(to)), _NDims{2}
  {
    if (_from_2 == nullptr || _to_2 == nullptr)
      throw std::runtime_error(
          "Reorderer must have non-nullptr curves to reorder");
    if (_from_2->dimensionLength() != _to_2->dimensionLength())
      throw std::runtime_error("SFCurves must have the same size");
  }
  reorderer(std::unique_ptr<sfc::sfcurve<3>>&& from,
            std::unique_ptr<sfc::sfcurve<3>>&& to)
      : _from_3(std::move(from)), _to_3(std::move(to)), _NDims{3}
  {
    if (_from_3 == nullptr || _to_3 == nullptr)
      throw std::runtime_error(
          "Reorderer must have non-nullptr curves to reorder");
    if (_from_3->dimensionLength() != _to_3->dimensionLength())
      throw std::runtime_error("SFCurves must have the same size");
  }

  // Run in-place re-ordering of some iterator pair using SFCs
  template <typename T>
  void reorder(T* data, unsigned long long length)
  {
    if (_NDims == 2 && _from_2 == _to_2) return;
    if (_NDims == 3 && _from_3 == _to_3) return;
    std::vector<bool> _moved_checklist;
    _moved_checklist.resize(length);
    auto count = 0;
    auto prevcount = count;

    auto start_boundary = data;
    while (start_boundary != data + length) {
      auto current = start_boundary;
      auto current_index = current - data;
      auto tempValue = *current;
      do {
        _moved_checklist[current_index] = true;
        sfc::size_t target_index;
        if (_NDims == 2)
          target_index =
              _to_2->coordsToDist(_from_2->distToCoords(current_index));
        else if (_NDims == 3)
          target_index =
              _to_3->coordsToDist(_from_3->distToCoords(current_index));
        if (target_index == current_index) continue;
        auto target = data + target_index;
        auto temp = tempValue;
        tempValue = *target;
        *target = temp;
        current = target;
        current_index = target_index;
      } while (!_moved_checklist[current_index]);
      // Move start_boundary to first element not moved
      auto next = std::find(std::begin(_moved_checklist),
                            std::end(_moved_checklist), false);
      start_boundary = data + (next - std::begin(_moved_checklist));
    }
  }

  void reorder_withtemporary_vardtype(std::uint8_t* data,
                                      unsigned long long lengthInBytes,
                                      int nBytesInDType)
  {
    if (lengthInBytes % nBytesInDType != 0)
      throw std::range_error("Data length is not divisible by data-type size");

    if (nBytesInDType == 2)
      reorder_withtemporary((std::uint16_t*)data,
                            lengthInBytes / sizeof(std::uint16_t));
    else if (nBytesInDType == 4)
      reorder_withtemporary((std::uint32_t*)data,
                            lengthInBytes / sizeof(std::uint32_t));
  }
  template <typename T>
  void reorder_withtemporary(T* data, unsigned long long length)
  {
    auto temp = new T[length];
    // Swap
    if (_NDims == 2) {
      for (auto i = 0ULL; i < length; i++) {
        auto from_val = _from_2->distToCoords(i);
        auto index = _to_2->coordsToDist(from_val);
        if (index >= length) {
          std::cout << "index=" << index << " >= length=" << length << std::endl
                    << "\tfrom=" << from_val[0] << "," << from_val[1] << ","
                    << from_val[2] << std::endl;
        }
        temp[index] = data[i];
      }
    } else if (_NDims == 3) {
      for (auto i = 0ULL; i < length; i++) {
        auto from_val = _from_3->distToCoords(i);
        auto index = _to_3->coordsToDist(from_val);
        if (index >= length) {
          std::cout << "index=" << index << " >= length=" << length << std::endl
                    << "\tfrom=" << from_val[0] << "," << from_val[1] << ","
                    << from_val[2] << "\n\ti=" << i << std::endl;
        }
        temp[index] = data[i];
      }
    }
    // auto itfrom = data;
    std::move(temp, temp + length, data);
    delete[] temp;
  }
};

std::unique_ptr<reorderer> make_reorderer(
    const sfc::size_t NDims, const sfc::size_t& dimlength,
    const sfc::main::sfcs::types& from_type,
    const sfc::main::sfcs::types& to_type)
{
  if (NDims == 2) {
    return std::make_unique<reorderer>(
        sfc::main::SFCToSFCurve<2>(from_type, dimlength),
        sfc::main::SFCToSFCurve<2>(to_type, dimlength));
  } else if (NDims == 3) {
    return std::make_unique<reorderer>(
        sfc::main::SFCToSFCurve<3>(from_type, dimlength),
        sfc::main::SFCToSFCurve<3>(to_type, dimlength));
  }
  return nullptr;
}
}  // namespace sfcc

#endif