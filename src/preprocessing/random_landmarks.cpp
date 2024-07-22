#include "random_landmarks.h"
#include "dijkstra.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

Random_lm_data Random_landmarks::preprocess(size_t lm_count, const Graph &graph,
                                            const Graph &graph_reverse) {
  std::cout << "Starting preprocessing..." << std::endl;

  std::vector<size_t> lm_indexes;
  std::vector<std::vector<uint32_t>> from_lm_distances;
  std::vector<std::vector<uint32_t>> to_lm_distances;

  lm_indexes.reserve(lm_count);
  from_lm_distances.reserve(lm_count);
  to_lm_distances.reserve(lm_count);

  Dijkstra dijkstra(graph);
  Dijkstra dijkstra_reverse(graph_reverse);

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<uint32_t> distribution(
      0, dijkstra.node_offsets.size() - 1);

  for (size_t i = 0; i < lm_count; ++i) {
    lm_indexes.push_back(i);

    uint32_t random_landmark = distribution(rng);
    /*std::cout << "Random landmark ID: " << random_landmark << "\n";*/

    std::vector<uint32_t> distances_forward =
        dijkstra.src_to_all(random_landmark);
    from_lm_distances.push_back(std::move(distances_forward));

    std::vector<uint32_t> distances_reverse =
        dijkstra_reverse.src_to_all(random_landmark);
    to_lm_distances.push_back(std::move(distances_reverse));
  }

  std::cout << "Preprocessing finished!" << std::endl;

  Random_lm_data lm_data;

  lm_data.lm_indexes = std::move(lm_indexes);
  lm_data.from_lm_distances = std::move(from_lm_distances);
  lm_data.to_lm_distances = std::move(to_lm_distances);

  return lm_data;
}
