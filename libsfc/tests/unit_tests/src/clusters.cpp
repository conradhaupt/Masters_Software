/**
 * @file clusters.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-05-12
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "libsfc/clusters.h"
#include <gtest/gtest.h>
#include <bitset>
#include <string>

namespace {
TEST(clusters, mergesClustersInConstructor)
{
  auto clusters = sfc::clusters<int>({{1, 4}, {7, 9}, {10, 15}, {20, 23}});
  std::vector<typename sfc::clusters<int>::cluster> expected_clusters = {
      {1, 4}, {7, 15}, {20, 23}};
  EXPECT_EQ(clusters.getClusters(), expected_clusters);
};

TEST(clusters, constructorIgnoresOrderingOfClusters)
{
  auto clusters_1 = sfc::clusters<int>({{1, 7}, {9, 10}, {15, 90}, {121, 122}});
  auto clusters_2 = sfc::clusters<int>({{1, 7}, {15, 90}, {9, 10}, {121, 122}});
  EXPECT_EQ(clusters_1.getClusters(), clusters_2.getClusters());
};

TEST(clusters, mergesClustersWhenValueIsInserted)
{
  auto clusters = sfc::clusters<int>({{0, 0}, {7, 8}, {19, 25}, {50, 55}});
  auto values_to_be_inserted = {9, 6, 67};
  std::vector<typename sfc::clusters<int>::cluster> expected_clusters = {
      {0, 0}, {6, 9}, {19, 25}, {50, 55}, {67, 67}};
  for (const auto& v : values_to_be_inserted) clusters.insert(v);
  EXPECT_EQ(clusters.getClusters(), expected_clusters);
};

TEST(clusters, clustersMergedWithTemplatedConstructor)
{
  std::array<int, 10> arr = {0, 1, 2, 3, 5, 6, 8, 9, 10};
  std::vector<typename sfc::clusters<int>::cluster> expected_clusters = {
      {0, 3}, {5, 6}, {8, 10}};
  auto clusters = sfc::clusters<int>(arr);
  EXPECT_EQ(clusters.getClusters(), expected_clusters);
}
};  // namespace