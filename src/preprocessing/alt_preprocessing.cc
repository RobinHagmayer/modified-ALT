#include "preprocessing/alt_preprocessing.h"

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

#include "algorithms/dijkstra.h"
#include "preprocessing/utils.h"

namespace modified_alt {
namespace preprocessing {

LandmarkDistanceVectors PreprocessRandom(
    const algorithms::Dijkstra &dijkstra_reverse, const size_t landmark_count,
    const size_t node_count) {
  LandmarkDistanceVectors landmark_distance_vectors{};
  landmark_distance_vectors.reserve(landmark_count);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>(node_count));

  std::vector<uint32_t> highly_reachable_distance_vector{
      CalculateHighlyReachableDistanceVector(dist, gen, dijkstra_reverse)};

  size_t remaining_landmarks{landmark_count};
  while (remaining_landmarks > 0) {
    uint32_t random_node{dist(gen)};

    if (highly_reachable_distance_vector.at(random_node) == UINT32_MAX) {
      continue;
    }

    landmark_distance_vectors.emplace_back(
        dijkstra_reverse.ComputeSSSP(random_node));

    --remaining_landmarks;
  }

  return landmark_distance_vectors;
}

}  // namespace preprocessing
}  // namespace modified_alt
