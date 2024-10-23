#include "algorithms/dijkstra.h"

#include <cstdint>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace algorithms {

using DijkstraPQ =
    std::priority_queue<std::pair<uint32_t, uint32_t>,
                        std::vector<std::pair<uint32_t, uint32_t>>,
                        std::greater<>>;

Dijkstra::Dijkstra(const graph::Graph graph)
    : offset_array_{graph.GetOffsetArray()},
      edge_array_{graph.GetEdgeArray()} {}

std::vector<uint32_t> Dijkstra::ComputeSSSP(const uint32_t source) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  DijkstraPQ pq{};

  distances.at(source) = 0;
  pq.push({0, source});

  while (!pq.empty()) {
    const uint32_t current_distance{pq.top().first};
    const uint32_t current_node{pq.top().second};
    pq.pop();

    // If the distance is greater than in the distances array then we already
    // visited this node.
    if (current_distance > distances.at(current_node)) {
      continue;
    }

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    // Check each neighbor for better distance.
    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        pq.push({distances.at(edge_to_neighbor.to), edge_to_neighbor.to});
      }
    }
  }

  return distances;
}

uint32_t Dijkstra::ComputeSPSP(const uint32_t source,
                               const uint32_t target) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  DijkstraPQ pq{};

  distances.at(source) = 0;
  pq.push({0, source});

  while (!pq.empty()) {
    const uint32_t current_distance{pq.top().first};
    const uint32_t current_node{pq.top().second};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    // If the distance is greater than in the distances array then we already
    // visited this node.
    if (current_distance > distances.at(current_node)) {
      continue;
    }

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    // Check each neighbor for better distance.
    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        pq.push({distances.at(edge_to_neighbor.to), edge_to_neighbor.to});
      }
    }
  }

  return UINT32_MAX;
}

uint32_t Dijkstra::ComputeSPSP(const uint32_t source, const uint32_t target,
                               uint32_t &visited_nodes) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  DijkstraPQ pq{};

  distances.at(source) = 0;
  pq.push({0, source});

  while (!pq.empty()) {
    const uint32_t current_distance{pq.top().first};
    const uint32_t current_node{pq.top().second};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    // If the distance is greater than in the distances array then we already
    // visited this node.
    if (current_distance > distances.at(current_node)) {
      continue;
    }

    ++visited_nodes;

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    // Check each neighbor for better distance.
    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        pq.push({distances.at(edge_to_neighbor.to), edge_to_neighbor.to});
      }
    }
  }

  return UINT32_MAX;
}

}  // namespace algorithms
}  // namespace modified_alt
