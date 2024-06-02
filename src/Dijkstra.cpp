#include "Dijkstra.h"
#include "Edge.h"

#include <climits>
#include <functional>
#include <optional>
#include <queue>
#include <utility>

DijkstraAllResult Dijkstra::src_to_all(int src) {
  using std::pair;
  using std::vector;

  DijkstraAllResult dijkstra_result = {false, std::nullopt, std::nullopt};

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
      Edge neighbor = edges_[i];

      if (distances[current_node_id] + neighbor.weight <
          distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] =
            distances[current_node_id] + neighbor.weight;
        pq.push({distances[neighbor.trg_id], neighbor.trg_id});
      }
    }
  }

  dijkstra_result.success = true;
  dijkstra_result.distances = distances;
  return dijkstra_result;
}

DijkstraSingleResult Dijkstra::src_to_trg(int src, int trg) {
  using std::pair;
  using std::vector;

  DijkstraSingleResult dijkstra_result = {false, std::nullopt, std::nullopt};

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
      dijkstra_result.success = true;
      dijkstra_result.cost = distances[trg];
      return dijkstra_result;
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
      Edge neighbor = edges_[i];

      if (distances[current_node_id] + neighbor.weight <
          distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] =
            distances[current_node_id] + neighbor.weight;
        pq.push({distances[neighbor.trg_id], neighbor.trg_id});
      }
    }
  }

  dijkstra_result.error_message = "Target was not found.";
  return dijkstra_result;
}
