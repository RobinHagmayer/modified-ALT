#include "new_ALT.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <sys/types.h>
#include <tuple>

uint32_t New_ALT::src_to_trg(uint32_t src_id, uint32_t trg_id,
                             uint32_t &nodes_checked) {
  using std::vector;
  using Triple = std::tuple<uint32_t, uint32_t, uint32_t>;

  auto cmp = [](const Triple &left, const Triple &right) {
    return std::get<0>(left) > std::get<0>(right);
  };

  // Initialize distances vector, visited array and priority queue
  uint32_t number_of_nodes = graph.node_offsets.size() - 1;
  vector<uint32_t> distances(number_of_nodes, UINT32_MAX);
  distances[src_id] = 0;
  std::priority_queue<Triple, std::vector<Triple>, decltype(cmp)> pq(cmp);
  pq.emplace(0, src_id, 0);
  uint32_t min_distance = UINT32_MAX;

  while (!pq.empty()) {
    // Get the closest node to the source
    Triple current = pq.top();
    pq.pop();

    uint32_t current_node_id = std::get<1>(current);
    uint32_t current_node_dist= std::get<2>(current);

    // Shortest path found. Early return
    if (current_node_id == trg_id) {
      return distances.at(trg_id);
    }

    // Skip nodes which won't be better
    if (current_node_dist > distances.at(current_node_id)) {
      continue;
    }

    nodes_checked++;

    // Check the neighbors of the current node
    uint32_t start = graph.node_offsets[current_node_id];
    uint32_t end = graph.node_offsets[current_node_id + 1];
    for (size_t i = start; i < end; ++i) {
      const Edge &neighbor = graph.edges[i];
      const uint32_t new_costs = distances[current_node_id] + neighbor.weight;

      if (new_costs < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_costs;
        uint32_t priority = new_costs + h(neighbor.trg_id, trg_id);
        pq.emplace(priority, neighbor.trg_id, new_costs);
      }
    }
  }

  return distances[trg_id];
}

uint32_t New_ALT::h(uint32_t node_id, uint32_t trg_id) {
  /*closests_from_lm (from lm to nodes, <-) stores for every node the pair
   * <from_closests_lm_to_node, dist>*/

  /*closests_to_lm (from nodes to lm, ->) stores for every node the pair
   * <to_closests_lm_from_node, dist>*/

  int32_t score = 0;

  // l_v^<- ; from lm to v
  uint32_t closests_lm_to_v = closests_from_lm.at(node_id).first;
  // l_t^-> ; from t to lm
  uint32_t closests_lm_from_t = closests_to_lm.at(trg_id).first;

  if (closests_lm_to_v == UINT32_MAX || closests_lm_from_t == UINT32_MAX ||
      closests_from_lm.at(node_id).second == UINT32_MAX ||
      closests_to_lm.at(trg_id).second == UINT32_MAX) {
    return 0;
  }

  // d(l_v^<-, l_t^->)
  uint32_t closests_lm_dist =
      dist_between_lms.at(closests_lm_to_v).at(closests_lm_from_t);

  if (closests_lm_dist == UINT32_MAX) {
    return 0;
  }

  // h(v)=d(l_v^<-, l_t^->) - d(l_v^<-, v) - d(t, l_t^->)
  score += closests_lm_dist;
  score -= closests_from_lm.at(node_id).second;
  score -= closests_to_lm.at(trg_id).second;

  if (score < 0) {
    return 0;
  }

  return static_cast<uint32_t>(score);
}
