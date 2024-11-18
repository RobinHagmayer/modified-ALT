#include "preprocessing/farthest.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <random>

#include "algorithms/dijkstra.h"
#include "graph/graph.h"
#include "preprocessing/data_types.h"
#include "preprocessing/utils.h"

namespace modified_alt {
namespace preprocessing {

LandmarkDistanceVectors SelectFarthestLandmarks(
    const graph::Graph &graph, const graph::Graph &graph_reverse,
    const size_t landmark_count) {
  using algorithms::Dijkstra;

  LandmarkDistanceVectors landmark_distance_vectors{};
  landmark_distance_vectors.reserve(landmark_count);

  Dijkstra dijkstra(graph);
  Dijkstra dijkstra_reverse(graph_reverse);

  std::vector<uint32_t> highly_reachable_distance_vector{
      preprocessing::CalculateHighlyReachableDistanceVector(graph, 98.0)};

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<uint32_t> distribution(
      0, static_cast<uint32_t>(graph.GetNumberOfNodes()));

  uint32_t random_node{};
  do {
    random_node = distribution(rng);
  } while (highly_reachable_distance_vector.at(random_node) == UINT32_MAX);

  auto random_node_distances{dijkstra.ComputeSSSP(random_node)};
  uint32_t first_landmark{FindFarthestNode(random_node_distances,
                                           highly_reachable_distance_vector)};
  // std::cout << "Landmark ID: " << first_landmark << std::endl;

  auto first_landmark_distances{dijkstra_reverse.ComputeSSSP(first_landmark)};
  // Copy first landmark distances for aggregate distances.
  auto minimal_distances(first_landmark_distances);
  landmark_distance_vectors.emplace_back(first_landmark_distances);

  size_t landmark_counter{1};
  while (landmark_counter < landmark_count) {
    uint32_t next_landmark{
        FindFarthestNode(minimal_distances, highly_reachable_distance_vector)};
    // std::cout << "Landmark ID: " << next_landmark << std::endl;

    auto next_landmark_distances{dijkstra_reverse.ComputeSSSP(next_landmark)};

    // Update minimum distance array.
    for (size_t i{0}; i < minimal_distances.size(); ++i) {
      if (minimal_distances.at(i) > next_landmark_distances.at(i)) {
        minimal_distances.at(i) = next_landmark_distances.at(i);
      }
    }

    landmark_distance_vectors.emplace_back(next_landmark_distances);
    ++landmark_counter;
  }

  return landmark_distance_vectors;
}

uint32_t FindFarthestNode(
    const std::vector<uint32_t> &random_node_distances,
    const std::vector<uint32_t> &highly_reachable_distance_vector) {
  uint32_t max{0};
  uint32_t max_landmark{};

  for (uint32_t i{0}; i < random_node_distances.size(); ++i) {
    if (highly_reachable_distance_vector.at(i) == UINT32_MAX) {
      continue;
    }

    uint32_t distance{random_node_distances.at(i)};

    if (distance != UINT32_MAX && distance > max) {
      max = distance;
      max_landmark = i;
    }
  }

  if (max == 0) {
    std::cerr << "No farthest node could be found." << std::endl;
    exit(EXIT_FAILURE);
  }

  return max_landmark;
}

}  // namespace preprocessing
}  // namespace modified_alt
