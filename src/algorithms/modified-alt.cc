#include "algorithms/modified-alt.h"

#include <cstdint>
#include <queue>
#include <vector>

#include "algorithms/alt.h"
#include "graph/graph.h"
#include "preprocessing/modified-alt_preprocessing.h"

namespace modified_alt {
namespace algorithms {

auto cmp2 = [](const PQElement &left, const PQElement &right) {
  return left.priority > right.priority;
};

using ModifiedAltPq =
    std::priority_queue<PQElement, std::vector<PQElement>, decltype(cmp2)>;

ModifiedAlt::ModifiedAlt(const graph::Graph graph,
                         const preprocessing::ModifiedAltData modified_alt_data)
    : offset_array_{graph.GetOffsetArray()},
      edge_array_{graph.GetEdgeArray()},
      distances_between_landmarks_{
          modified_alt_data.distances_between_landmarks},
      from_landmark_to_nodes_{modified_alt_data.from_landmark_to_nodes},
      from_nodes_to_landmark_{modified_alt_data.from_nodes_to_landmark} {}

uint32_t ModifiedAlt::ComputeSPSP(const uint32_t source,
                                  const uint32_t target) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  ModifiedAltPq pq(cmp2);

  distances.at(source) = 0;
  pq.push({0, source, 0});

  while (!pq.empty()) {
    const uint32_t current_node{pq.top().node};
    const uint32_t current_distance{pq.top().distance};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    if (current_distance > distances.at(current_node)) {
      continue;
    }

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        const uint32_t priority{new_distance + h(edge_to_neighbor.to, target)};
        pq.push(
            {priority, edge_to_neighbor.to, distances.at(edge_to_neighbor.to)});
      }
    }
  }

  return UINT32_MAX;
}

uint32_t ModifiedAlt::ComputeSPSP(const uint32_t source, const uint32_t target,
                                  uint32_t &visited_nodes) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  ModifiedAltPq pq(cmp2);

  distances.at(source) = 0;
  pq.push({0, source, 0});

  while (!pq.empty()) {
    const uint32_t current_node{pq.top().node};
    const uint32_t current_distance{pq.top().distance};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    if (current_distance > distances.at(current_node)) {
      continue;
    }
    ++visited_nodes;

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        const uint32_t priority{new_distance + h(edge_to_neighbor.to, target)};
        pq.push(
            {priority, edge_to_neighbor.to, distances.at(edge_to_neighbor.to)});
      }
    }
  }

  return UINT32_MAX;
}

inline uint32_t ModifiedAlt::h(const uint32_t node,
                               const uint32_t target) const {
  long score{0};

  // l_v^<- ; from landmark to node
  uint32_t nearest_landmark_index_from_landmark_to_node{
      from_landmark_to_nodes_.at(node).index};
  // l_t^-> ; from target to landmark
  uint32_t nearest_landmark_index_from_target_to_landmark{
      from_nodes_to_landmark_.at(target).index};

  if (nearest_landmark_index_from_landmark_to_node == UINT32_MAX ||
      nearest_landmark_index_from_target_to_landmark == UINT32_MAX ||
      from_landmark_to_nodes_.at(node).cost == UINT32_MAX ||
      from_nodes_to_landmark_.at(target).cost == UINT32_MAX) {
    return 0;
  }

  // d(l_v^<-, l_t^->)
  uint32_t distance_between_landmarks{
      distances_between_landmarks_
          .at(nearest_landmark_index_from_landmark_to_node)
          .at(nearest_landmark_index_from_target_to_landmark)};

  if (distance_between_landmarks == UINT32_MAX) {
    return 0;
  }

  // h(v)=d(l_v^<-, l_t^->) - d(l_v^<-, v) - d(t, l_t^->)
  score += distance_between_landmarks;
  score -= from_landmark_to_nodes_.at(node).cost;
  score -= from_nodes_to_landmark_.at(target).cost;
  // std::cout << score << '\n';

  if (score < 0) {
    return 0;
  }

  return static_cast<uint32_t>(score);
}

}  // namespace algorithms
}  // namespace modified_alt
