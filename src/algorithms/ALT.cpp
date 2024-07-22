#include "ALT.h"

#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <tuple>

uint32_t ALT::src_to_trg(uint32_t src_id, uint32_t trg_id,
                         uint32_t &nodes_checked, size_t lm_count) {
  using std::vector;
  using Triple = std::tuple<uint32_t, uint32_t, uint32_t>;

  auto cmp = [](const Triple &left, const Triple &right) {
    return std::get<0>(left) > std::get<0>(right);
  };

  // Initialize distances vector, visited array and priority queue
  uint32_t number_of_nodes = node_offsets.size() - 1;
  vector<uint32_t> distances(number_of_nodes, UINT32_MAX);
  distances[src_id] = 0;
  std::priority_queue<Triple, std::vector<Triple>, decltype(cmp)> pq(cmp);
  pq.emplace(0, src_id, 0);

  while (!pq.empty()) {
    // Get the closest node to the source
    Triple current = pq.top();
    pq.pop();

    uint32_t current_node_id = std::get<1>(current);
    uint32_t current_node_dist= std::get<2>(current);

    // Target found. Early return
    if (current_node_id == trg_id) {
      return distances[trg_id];
    }

    // Skip nodes which won't be better
    if (current_node_dist > distances.at(current_node_id)) {
      continue;
    }

    nodes_checked++;

    // Check the neighbors of the current node
    uint32_t start = node_offsets[current_node_id];
    uint32_t end = node_offsets[current_node_id + 1];
    for (size_t i = start; i < end; ++i) {
      const Edge &neighbor = edges[i];
      const uint32_t new_costs = distances[current_node_id] + neighbor.weight;

      if (new_costs < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_costs;
        int32_t priority = new_costs + h(neighbor.trg_id, trg_id, lm_count);
        pq.emplace(priority, neighbor.trg_id, new_costs);
      }
    }
  }

  return distances[trg_id];
}

int32_t ALT::h(uint32_t node_id, uint32_t trg_id, size_t lm_count) {
  int32_t max = 0;

  for (size_t i = 0; i < lm_count; ++i) {
    if (to_lm_distances[i][node_id] == UINT32_MAX ||
        to_lm_distances[i][trg_id] == UINT32_MAX) {
      continue;
    }
    int32_t dist = to_lm_distances[i][node_id] - to_lm_distances[i][trg_id];

    if (dist > max) {
      max = dist;
    }
  }

  if (max < 0) {
    return 0;
  }

  return max;
}
