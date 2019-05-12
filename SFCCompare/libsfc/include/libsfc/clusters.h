/**
 * @file cluster.h
 * @author Conrad Haupt (conradjhaupt@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-11-01
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef SFC_CLUSTER_H
#define SFC_CLUSTER_H

#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <vector>
#include "sfcdef.h"

namespace sfc {
template <typename _Tp>
class clusters
{
 public:
  using cluster = std::array<_Tp, 2>;

  clusters() {}

  template <typename _TpContainer>
  clusters(const _TpContainer d)
  {
    // Check that container has the correct value type
    static_assert(std::is_same<typename _TpContainer::value_type, _Tp>::value);
    // If d is empty, do nothing
    if (d.size() == 0) return;
    // Copy and sort d
    auto _d = d;
    std::sort(std::begin(_d), std::end(_d));

    // Initialize first cluster, then process all elements
    auto curr_cluster = cluster({{*std::begin(_d), *std::begin(_d)}});
    for (const auto& x : _d) {
      if (x == curr_cluster[1] + 1) {
        curr_cluster[1] = x;
      } else if (x > curr_cluster[1] + 1) {
        _clusters.push_back(curr_cluster);
        curr_cluster[0] = x;
        curr_cluster[1] = x;
      }
    }
    _clusters.push_back(curr_cluster);
  }

  clusters(const std::initializer_list<std::array<_Tp, 2>>& c) : _clusters{c}
  {
    mergeClusters();
  }

  ~clusters() {}

  template <class CharT, class Traits>
  std::basic_ostream<CharT, Traits> operator<<(
      std::basic_ostream<CharT, Traits>&& os)
  {
    auto first = true;
    for (const auto& c : _clusters) {
      if (!first) os << " -> ";
      os << "[" << c.first << ", " << c.second << "]";
      first = false;
    }
  }

  std::vector<cluster> getClusters() const { return _clusters; }

 private:
  std::vector<cluster> _clusters;

  bool clustersCanBeMerged(const cluster& c1, const cluster& c2)
  {
    std::array<_Tp, 4> combined_cluster_values;
    std::copy(std::begin(c1), std::end(c1),
              std::begin(combined_cluster_values));
    std::copy(std::begin(c2), std::end(c2),
              std::begin(combined_cluster_values) + 2);

    auto max = std::max_element(std::begin(combined_cluster_values),
                                std::end(combined_cluster_values));
    auto min = std::min_element(std::begin(combined_cluster_values),
                                std::end(combined_cluster_values));

    auto max_dist = *max - *min + 1;
    auto sum_dist = (std::get<1>(c1) - std::get<0>(c1) + 1) +
                    (std::get<1>(c2) - std::get<0>(c2) + 1);
    return max_dist <= sum_dist;
  }

  void mergeClusters()
  {
    std::for_each(std::begin(_clusters), std::end(_clusters),
                  [](auto& c) { std::sort(std::begin(c), std::end(c)); });
    std::sort(std::begin(_clusters), std::end(_clusters));
    // Move through each cluster
    auto end = std::end(_clusters);
    for (auto it = std::begin(_clusters); it != end;) {
      // Get an iterator to the next cluster stored
      auto next_it = it;
      std::advance(next_it, 1);

      // Verify that we aren't going off the edge of the clusters vector
      if (next_it == end) {
        it++;
        continue;
      }

      // Continue if the next cluster cannot be merged with the current
      if (!clustersCanBeMerged(*it, *next_it)) {
        it++;
        continue;
      }

      // Set the current cluster upper bound to the max of the merged result
      std::get<1>(*it) = std::max(std::get<1>(*it), std::get<1>(*next_it));

      // Update 'it' and 'end'
      _clusters.erase(next_it);
      end = std::end(_clusters);
    }
  }

 public:
  void insert(const _Tp& value)
  {
    bool valueCanExpandCluster = false;
    bool returnFromFunction = false;
    for (auto& c : _clusters) {
      if (std::get<0>(c) - 1 == value) {
        // If the current cluster must extend downwards
        std::get<0>(c) = value;
        valueCanExpandCluster = true;
        returnFromFunction = true;
      } else if (std::get<1>(c) + 1 == value) {
        // If the current cluster must extend upwards
        std::get<1>(c) = value;
        valueCanExpandCluster = true;
        returnFromFunction = true;
      } else if (std::get<0>(c) <= value && std::get<1>(c) >= value) {
        // If the curent cluster already contains that value
        returnFromFunction = true;
      }

      if (valueCanExpandCluster) mergeClusters();
      if (returnFromFunction) return;
    }
    // The current value does not exist in a cluster
    _clusters.push_back({value, value});
  }

  std::vector<cluster> getclusters() const { return _clusters; }
  sfc::size_t count() const { return _clusters.size(); }

};  // namespace sfc
};  // namespace sfc
#endif