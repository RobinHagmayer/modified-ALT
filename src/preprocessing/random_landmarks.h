#pragma once

#include "dijkstra.h"

#include <unordered_map>
#include <vector>

class Random_landmarks {
public:
  static void select_landmarks(const int number_of_landmarks,
                               const std::vector<int> &node_offsets,
                               std::vector<int> &landmarks);
  static void preprocess_landmarks(
      Dijkstra &dijkstra, const std::vector<int> &landmarks,
      std::unordered_map<int, std::vector<int>> &landmark_distances);
};
