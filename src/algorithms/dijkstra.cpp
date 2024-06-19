#include "dijkstra.h"
#include "edge.h"

#include <climits>
#include <queue>

void Dijkstra::src_to_all(int src, std::vector<int> &distances) {
  using std::pair;
  using std::vector;

  distances[src] = 0;
  std::priority_queue<pair<int, int>, vector<pair<int, int>>,
                      std::greater<pair<int, int>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_dist = pq.top().first;
    int current_node_id = pq.top().second;
    pq.pop();

    // TODO: remove visited array but rather check distances in pq and dist
    if (current_node_dist > distances[current_node_id]) {
      continue;
    }

    // Check the neighbors of the current node
    int start = node_offsets_[current_node_id];
    int end = node_offsets_[current_node_id + 1];
    for (int i = start; i < end; i++) {
      const Edge &neighbor = edges_[i];

      if (distances[current_node_id] + neighbor.weight <
          distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] =
            distances[current_node_id] + neighbor.weight;
        pq.push({distances[neighbor.trg_id], neighbor.trg_id});
      }
    }
  }
}

int Dijkstra::src_to_trg(int src, int trg) {
  using std::pair;
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<int> distances(number_of_nodes, INT_MAX);
  distances[src] = 0;
  std::priority_queue<pair<int, int>, vector<pair<int, int>>,
                      std::greater<pair<int, int>>>
      pq;
  pq.push({0, src});
  int nodes_checked = 0;

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_dist = pq.top().first;
    int current_node_id = pq.top().second;
    pq.pop();

    // Target found. Early return
    if (current_node_id == trg) {
      std::cout << "Nodes checked: " << nodes_checked << std::endl;
      return distances[trg];
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
      const int new_cost = distances[current_node_id] + neighbor.weight;

      if (new_cost < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_cost;
        pq.push({new_cost, neighbor.trg_id});
      }
    }
  }

  // Target node was not found
  return -1;
}
