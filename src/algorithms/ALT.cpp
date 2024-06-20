#include "ALT.h"

#include <climits>
#include <cstdlib>
#include <functional>
#include <limits>
#include <queue>
#include <cmath>

int ALT::src_to_trg(int src_id, int trg_id, int &nodes_checked) {
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<bool> visited(number_of_nodes, false);
  vector<int> distances(number_of_nodes, std::numeric_limits<int>::max());
  distances[src_id] = 0;
  std::priority_queue<std::pair<int, int>, vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
  pq.push({0, src_id});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_id = pq.top().second;
    pq.pop();

    // Target found. Early return
    if (current_node_id == trg_id) {
      return distances[trg_id];
    }

    if (visited[current_node_id]) {
      continue;
    }

    visited[current_node_id] = true;
    nodes_checked++;

    // Check the neighbors of the current node
    int start = node_offsets_[current_node_id];
    int end = node_offsets_[current_node_id + 1];
    for (int i = start; i < end; i++) {
      const Edge &neighbor = edges_[i];
      const int new_costs = distances[current_node_id] + neighbor.weight;

      if (new_costs < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_costs;
        int priority = new_costs + h(neighbor.trg_id, trg_id);
        pq.push({priority, neighbor.trg_id});
      }
    }
  }

  return -1;
}

int ALT::h(int node_id, int trg_id) {
  int max = 0;

  for (const auto &[landmark, distances] : landmark_distances_reverse_) {
    /*if (distances[node_id] == INT_MAX || distances[trg_id] == INT_MAX) {*/
    /*  std::cout << "Landmark " << landmark << " has d(v,l) = INT_MAX" << std::endl;*/
    /*}*/
    int dist = std::abs(distances[node_id] - distances[trg_id]);

    if (dist > max) {
      max = dist;
    }
  }

  return max;
}

int ALT::h2(int node_id, int trg_id) {
  int max_forward = 0;
  int max_reverse = 0;

  for (const auto &[landmark, distances_forward] : landmark_distances_forward_) {
    auto distances_reverse = landmark_distances_reverse_.at(landmark);

    int dist_forward = std::abs(distances_forward[trg_id] - distances_forward[node_id]);
    int dist_reverse = std::abs(distances_reverse[node_id] - distances_reverse[trg_id]);

    if (dist_forward > max_forward) {
      max_forward = dist_forward;
    }

    if(dist_reverse > max_reverse) {
      max_reverse = dist_reverse;
    }
  }

  if (max_forward > max_reverse) {
    return max_forward;
  }

  return max_reverse;
}
