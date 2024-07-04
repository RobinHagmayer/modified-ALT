#pragma once

#include "dijkstra.h"

#include <unordered_map>
#include <vector>

class Farthest {
public:
  static void preprocess(
      const int &number_of_landmarks, Dijkstra &dijkstra_forward,
      Dijkstra &dijkstra_reverse,
      std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
      std::unordered_map<int, std::vector<int>> &landmark_distances_reverse);
  static void serialize(
      const std::string &file_path,
      const std::unordered_map<int, std::vector<int>> &landmark_distances);

  static void
  deserialize(const std::string &file_path,
              std::unordered_map<int, std::vector<int>> &landmark_distances);
};
