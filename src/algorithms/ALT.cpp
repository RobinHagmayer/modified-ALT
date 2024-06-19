#include "ALT.h"

#include <limits>
#include <queue>

struct PQ_struct {
  int priority;
  int node_id;
  int dist_to_node;

  PQ_struct(int p, int nid, int dtn)
      : priority(p), node_id(nid), dist_to_node(dtn) {}
};

struct Compare_pq_struct {
  bool operator()(const PQ_struct &lhs, const PQ_struct &rhs) {
    return lhs.dist_to_node > rhs.dist_to_node;
  }
};

int ALT::src_to_trg(int src_id, int trg_id) {
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<int> distances(number_of_nodes, std::numeric_limits<int>::max());
  distances[src_id] = 0;
  std::priority_queue<PQ_struct, vector<PQ_struct>, Compare_pq_struct> pq;
  pq.push(PQ_struct(0, src_id, 0));
  int nodes_checked = 0;

  while (!pq.empty()) {
    // Get the closest node to the source
    PQ_struct current = pq.top();
    pq.pop();
    int current_node_id = current.node_id;
    int current_node_dist = current.dist_to_node;

    // Target found. Early return
    if (current_node_id == trg_id) {
      std::cout << "Nodes checked: " << nodes_checked << std::endl;
      return distances[trg_id];
    }

    // Check if we visited this node already.
    // If we saw this node already then it has a greater distance in the pq than
    // in the distances array.
    if (current_node_dist > distances[current_node_id]) {
      continue;
    }
    nodes_checked++;

    // Check the neighbors of the current node
    int start = node_offsets_[current_node_id];
    int end = node_offsets_[current_node_id + 1];
    for (int i = start; i < end; i++) {
      const Edge &neighbor = edges_[i];

      int new_costs = distances[current_node_id] + neighbor.weight;
      if (new_costs < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_costs;
        int priority = new_costs + h(current_node_id, trg_id);
        pq.push(PQ_struct(priority, neighbor.trg_id, new_costs));
      }
    }
  }

  return -1;
}

int ALT::h(int node_id, int trg_id) {
  int max = 0;

  for (const auto &[landmark, landmark_distances] : landmark_distances_) {
    int dist = landmark_distances[node_id] - landmark_distances[trg_id];
    if (dist > max) {
      max = dist;
    }
  }

  return max;
}
