#pragma once

#include <unordered_map>
#include <vector>

class New {
public:
  static void preprocess(const int &number_of_nodes,
                         const std::unordered_map<int, std::vector<int>>
                             &landmark_distances_forward,
                         const std::unordered_map<int, std::vector<int>>
                             &landmark_distances_reverse);
  static int get_landmark_distance(
      const int &l1, const int &l2,
      std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
      const std::unordered_map<int, std::vector<int>>
          &landmark_distances_reverse);
};
