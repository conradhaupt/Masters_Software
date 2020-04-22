/**
 * @file sfccexcept.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-08-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFCC_EXCEPT_H
#define SFCC_EXCEPT_H

#include <exception>
#include <string>
namespace sfc::sfcc {
class exception : public std::exception
{};

class DataAlreadyCompressed : public exception
{
 private:
  const compression_t _comp;
  std::string _what;

 public:
  DataAlreadyCompressed(const compression_t& comp = compression_t::NONE)
      : _comp(comp)
  {
    _what = std::string("Data is already compressed") +
            (_comp == compression_t::NONE ? "" : " as " + tostring(_comp));
  }
  virtual const char* what() const noexcept { return _what.c_str(); }
};

}  // namespace sfc::sfcc

#endif