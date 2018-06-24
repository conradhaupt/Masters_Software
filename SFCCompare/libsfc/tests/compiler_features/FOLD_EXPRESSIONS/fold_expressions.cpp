
#include <iostream>

template <typename T, typename... Ts>
T fold_add(Ts... ts) {
  return (ts + ...);
}

template <typename T, typename... Ts>
T fold_minus(Ts... ts) {
  return (ts - ...);
}

template <typename T, typename... Ts>
T fold_multiply(Ts... ts) {
  return (ts * ...);
}

template <typename T, typename... Ts>
T fold_divide(Ts... ts) {
  return (... / ts);
}

template <typename T, typename... Ts>
T fold_bitand(Ts... ts) {
  return (ts & ...);
}

template <typename T, typename... Ts>
T fold_bitor(Ts... ts) {
  return (ts | ...);
}

template <typename T, typename... Ts>
T fold_bitxor(Ts... ts) {
  return (ts ^ ...);
}

template <typename T, typename... Ts>
void fold_pipe(Ts... ts) {
  (::std::cout << ... << ts);
}

int main(void) {
  std::cout << "Values:";
  fold_pipe<int>(1, 2, 3, 4, 5, 6, 7, 8, 9);
  std::cout << std::endl;
  std::cout << "fold_add:" << fold_add<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_minus:" << fold_minus<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_multiply:" << fold_multiply<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_divide:" << fold_divide<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_bitand:" << fold_bitand<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_bitor:" << fold_bitor<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;
  std::cout << "fold_bitxor:" << fold_bitxor<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
            << std::endl;

  return 0;
}
