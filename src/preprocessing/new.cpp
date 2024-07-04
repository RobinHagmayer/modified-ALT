#include "new.h"

#include <climits>

void New::preprocess(
    const int &number_of_nodes,
    const std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
    const std::unordered_map<int, std::vector<int>>
        &landmark_distances_reverse) {
  for (int i = 0; i < number_of_nodes; i++) {
    int min_dist_to_landmark = INT_MAX;
    int to_landmark = -1;
    int min_dist_from_landmark = INT_MAX;
    int from_landmark = -1;

    for (auto pair : landmark_distances_forward) {
      int dist_to_landmark = pair.second[i];
      int dist_from_landmark = landmark_distances_reverse.at(pair.first)[i];

      if (dist_to_landmark < min_dist_to_landmark) {
        min_dist_to_landmark = dist_to_landmark;
        to_landmark = pair.first;
      }

      if (dist_from_landmark < min_dist_from_landmark) {
        min_dist_from_landmark = dist_from_landmark;
        from_landmark = pair.first;
      }
    }

    // TODO: store nearest landmarks
  }
}

int New::get_landmark_distance(
    const int &l1, const int &l2,
    std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
    const std::unordered_map<int, std::vector<int>>
        &landmark_distances_reverse) {
  return landmark_distances_forward[l1][l2];
}
