#include "dijkstra.h"
#include "edge.h"

#include <climits>
#include <functional>
#include <queue>
#include <utility>

void Dijkstra::src_to_all(int src) {
  using std::pair;
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<int> distances(number_of_nodes, INT_MAX);
  distances[src] = 0;
  vector<bool> visited(number_of_nodes, false);
  std::priority_queue<pair<int, int>, vector<pair<int, int>>,
                      std::greater<pair<int, int>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_id = pq.top().second;
    pq.pop();

    // TODO: remove visited array but rather check distances in pq and dist
    if (visited[current_node_id]) {
      continue;
    }
    visited[current_node_id] = true;

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

  // TODO: return
}

int Dijkstra::src_to_trg(int src, int trg) {
  using std::pair;
  using std::vector;

  // Initialize distances vector, visited array and priority queue
  int number_of_nodes = node_offsets_.size() - 1;
  vector<int> distances(number_of_nodes, INT_MAX);
  distances[src] = 0;
  vector<bool> visited(number_of_nodes, false);
  std::priority_queue<pair<int, int>, vector<pair<int, int>>,
                      std::greater<pair<int, int>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    int current_node_id = pq.top().second;
    pq.pop();

    // Target found. Early return
    if (current_node_id == trg) {
      return distances[trg];
    }

    // Check if we visited this node already.
    // We need to do this because of lazy insert into the pq
    if (visited[current_node_id]) {
      continue;
    }
    visited[current_node_id] = true;

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

  // Target node was not found
  return -1;
}
