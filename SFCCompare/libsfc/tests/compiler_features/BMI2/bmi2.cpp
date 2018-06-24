#include <immintrin.h>
#include <stdio.h>
#include <iostream>

int main(void) {
  __builtin_cpu_init();
  if (__builtin_cpu_supports("bmi2")) {
    printf("BMI2 supported \n");
  }

#if defined(__BMI2__)
  printf("__BMI2__ defined \n");
#else
  printf("__BMI2__ not defined \n");
#endif
  unsigned long a = 3033133307;
  unsigned long long b = -1UL << 32;
  ::std::cout << a << std::endl;
  ::std::cout << b << std::endl;
  ::std::cout << _pdep_u64(a, b) << std::endl;
  return 0;
}
