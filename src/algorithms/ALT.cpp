#include "ALT.h"

#include <limits>
#include <queue>

int ALT::src_to_trg(int src_id, int trg_id) {
  using std::pair;
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<int> distances(number_of_nodes, std::numeric_limits<int>::max());
  distances[src_id] = 0;
  std::priority_queue<pair<int, int>, vector<pair<int, int>>,
                      std::greater<pair<int, int>>>
      pq;
  pq.push({0, src_id});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_dist = pq.top().first;
    int current_node_id = pq.top().second;
    pq.pop();

    // Target found. Early return
    if (current_node_id == trg_id) {
      return distances[trg_id];
    }

    // Check if we visited this node already.
    // If we saw this node already then it has a greater distance in the pq than
    // in the distances array.
    if (current_node_dist > distances[current_node_id]) {
      continue;
    }

    // Check the neighbors of the current node
    int start = node_offsets_[current_node_id];
    int end = node_offsets_[current_node_id + 1];
    for (int i = start; i < end; i++) {
      const Edge &neighbor = edges_[i];

      int new_costs = distances[current_node_id] + neighbor.weight;
      if (new_costs < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_costs;
        int priority = new_costs + h(current_node_id, trg_id);
        pq.push({priority, neighbor.trg_id});
      }
    }
  }

  return -1;
}

int ALT::h(int node_id, int trg_id) {
  // Im Paper wird max_l-in-L(d(v,l)-d(t,l),d(l,t)-d(l,v)) genommen
  int min = std::numeric_limits<int>::max();

  for (const auto &[landmark, landmark_distances] : landmark_distances_) {
    int dist = landmark_distances[node_id] - landmark_distances[trg_id];
    if (dist < min) {
      min = dist;
    }
  }

  return min;
}
