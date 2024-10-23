#include "preprocessing/utils.h"

#include <cstdint>
#include <random>
#include <vector>

#include "algorithms/dijkstra.h"
#include "graph/graph.h"

namespace modified_alt {
namespace preprocessing {

std::vector<uint32_t> CalculateHighlyReachableDistanceVector(
    const graph::Graph &graph_reverse, const double reachability_percentage) {
  size_t node_count{graph_reverse.GetNumberOfNodes()};
  const algorithms::Dijkstra dijkstra_reverse(graph_reverse);
  std::vector<uint32_t> highly_reachable_distance_vector{};
  highly_reachable_distance_vector.reserve(node_count);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>(node_count));

  bool done{false};
  while (!done) {
    uint32_t random_node{dist(gen)};

    highly_reachable_distance_vector =
        dijkstra_reverse.ComputeSSSP(random_node);

    uint32_t count_unreachable{0};
    for (const uint32_t &d : highly_reachable_distance_vector) {
      if (d == UINT32_MAX) {
        ++count_unreachable;
      }
    }

    double numerator{static_cast<double>(count_unreachable)};
    double denominator{
        static_cast<double>(highly_reachable_distance_vector.size())};
    double unreachable_percentage{numerator / denominator};

    double calculated_reachability{(1.0 - unreachable_percentage) * 100};
    if (calculated_reachability < reachability_percentage) {
      continue;
    }

    done = true;
  }

  return highly_reachable_distance_vector;
}

}  // namespace preprocessing
}  // namespace modified_alt
