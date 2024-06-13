#include "random_landmarks.h"

#include <limits>
#include <random>
#include <unordered_map>

void Random_landmarks::select_landmarks(const int number_of_landmarks,
                                        const std::vector<int> &node_offsets,
                                        std::vector<int> &landmarks) {
  int number_of_nodes = node_offsets.size() - 1;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  for (int i = 0; i < number_of_landmarks; i++) {
    landmarks.push_back(distribution(rng));
  }
}

// Graph in reverse
void Random_landmarks::preprocess_landmarks(
    Dijkstra &dijkstra, const std::vector<int> &landmarks,
    std::unordered_map<int, std::vector<int>> &landmark_distances) {

  for (int landmark : landmarks) {
    std::vector<int> distances(dijkstra.node_offsets_.size() - 1,
                               std::numeric_limits<int>::max());
    dijkstra.src_to_all(landmark, distances);
    landmark_distances[landmark] = distances;
  }
}
