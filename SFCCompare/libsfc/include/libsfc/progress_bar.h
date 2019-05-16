/**
 * @file progress_bar.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-05-14
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SFC_PROGRESS_BAR_H
#define SFC_PROGRESS_BAR_H

#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>

namespace sfc {
namespace cli {
class progressbar
{
 private:
  unsigned long long _max;
  unsigned long long _current;
  int _prev_percentage;
  char _bar_char;
  int _max_width;
  int _perc_width;
  int _bar_width;
  unsigned __prev_width;

  std::mutex _mtx;

 public:
  progressbar(unsigned long long max, char bar_char = '#', int bar_width = 20)
      : _max(max),
        _current(0),
        _bar_char(bar_char),
        _bar_width(bar_width),
        _prev_percentage(0),
        _perc_width(3)
  {
    _max_width = _bar_width + 1 + _perc_width;
  }

  ~progressbar()
  {
    updateCLI();
    std::cout << std::endl;
  }

  progressbar(const progressbar& pg) = delete;

  void addProgress()
  {
    std::lock_guard<std::mutex> lock(_mtx);
    _current++;
    auto _curr_percentage =
        static_cast<int>(static_cast<double>(_current * 100) / _max);
    if (_curr_percentage > _prev_percentage) {
      updateCLI();
      _prev_percentage = _curr_percentage;
    }
    // __print_count = ++__print_count % _n_before_print;
    // std::cout << __print_count << std::endl;
    // else
    //   std::cout << __print_count << std::endl;
  }

  void updateCLI() const
  {
    std::string output = "[";
    auto n_bar = (_bar_width - 2.0) * _current / _max;
    for (auto i = 0; i < _bar_width - 2; i++)
      output += i <= n_bar ? _bar_char : ' ';
    output += ']';
    std::cout << "\r" << output << " " << std::setw(_perc_width)
              << std::setfill(' ') << _prev_percentage << "%" << std::flush;
  }
};
};  // namespace cli
};  // namespace sfc

#endif /* PROGRESS_BAR_H */
