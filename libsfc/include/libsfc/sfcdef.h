//
// Created by conrad on 2018/06/05.
//

#ifndef SFC_SFCDEF_H
#define SFC_SFCDEF_H

#include <cstddef>

namespace sfc {
#ifdef __LIBSFC_BMI2__
inline const bool BMI2_SUPPORTED = []() {
  __builtin_cpu_init();
  return __builtin_cpu_supports("bmi2");
}();
#else
inline constexpr bool BMI2_SUPPORTED = false;
#endif
using size_t = std::size_t;

};  // namespace sfc

#endif
