#include "ALT.h"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <queue>

int ALT::src_to_trg(int src_id, int trg_id, int &nodes_checked,
                    const std::vector<std::vector<int>> &landmark_distances) {
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<bool> visited(number_of_nodes, false);
  vector<int> distances(number_of_nodes, std::numeric_limits<int>::max());
  distances[src_id] = 0;
  std::priority_queue<std::pair<int, int>, vector<std::pair<int, int>>,
                      std::greater<std::pair<int, int>>>
      pq;
  pq.push({0, src_id});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_id = pq.top().second;
    int priority = pq.top().first;
    pq.pop();

    /*if (distances[trg_id] <= priority) {*/
    /*  return distances[trg_id];*/
    /*}*/
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
        int priority =
            new_costs + h(neighbor.trg_id, trg_id, landmark_distances);
        pq.push({priority, neighbor.trg_id});
      }
    }
  }

  return -1;
}

int ALT::h(int node_id, int trg_id,
           const std::vector<std::vector<int>> &landmark_distances) {
  int max = 0;
  int number_of_landmarks = 3;

  for (int i = 0; i < number_of_landmarks; i++) {
    if (landmark_distances[i][node_id] == INT_MAX || landmark_distances[i][trg_id] == INT_MAX) {
      continue;
    }
    int dist = landmark_distances[i][node_id] - landmark_distances[i][trg_id];

    if (dist > max) {
      max = dist;
    }
  }

  return max;
}
