#pragma once

#include "dijkstra.h"

#include <vector>

class Farthest {
public:
  static void preprocess(
      const int &number_of_landmarks, Dijkstra &dijkstra_forward,
      Dijkstra &dijkstra_reverse,
      std::vector<std::vector<int>> &landmark_distances_forward,
      std::vector<std::vector<int>> &landmark_distances_reverse);

  static int find_farthest_node(const std::vector<int> &random_node_distances);

  static std::vector<bool> check_reachability(Dijkstra &dijkstra_forward,
                                              Dijkstra &dijkstra_reverse,
                                              int node_id);

  static void serialize(
      const std::string &file_path,
      const std::vector<std::vector<int>> &landmark_distances);

  static void
  deserialize(const std::string &file_path,
              std::vector<std::vector<int>> &landmark_distances);
};
