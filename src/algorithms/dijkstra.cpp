#include "dijkstra.h"

#include <climits>
#include <cstdint>
#include <queue>
#include <vector>

void Dijkstra::src_to_all(uint32_t src, std::vector<uint32_t> &distances) {
  using std::pair;
  using std::vector;

  // We cant' lazy update in here because we need to set the distances to infinity
  distances.resize(node_offsets.size() - 1, UINT32_MAX);

  distances[src] = 0;
  std::priority_queue<pair<uint32_t, uint32_t>,
                      vector<pair<uint32_t, uint32_t>>,
                      std::greater<pair<uint32_t, uint32_t>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    uint32_t current_node_dist = pq.top().first;
    uint32_t current_node_id = pq.top().second;
    pq.pop();

    if (current_node_dist > distances[current_node_id]) {
      continue;
    }

    // Check the neighbors of the current node
    uint32_t start = node_offsets[current_node_id];
    uint32_t end = node_offsets[current_node_id + 1];
    for (uint32_t i = start; i < end; ++i) {
      const Edge &neighbor = edges[i];

      if (distances[current_node_id] + neighbor.weight <
          distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] =
            distances[current_node_id] + neighbor.weight;
        pq.push({distances[neighbor.trg_id], neighbor.trg_id});
      }
    }
  }
}

std::vector<uint32_t> Dijkstra::src_to_all(uint32_t src) {
  using std::pair;
  using std::vector;

  std::vector<uint32_t> distances(node_offsets.size() - 1, UINT32_MAX);

  distances[src] = 0;
  std::priority_queue<pair<uint32_t, uint32_t>,
                      vector<pair<uint32_t, uint32_t>>,
                      std::greater<pair<uint32_t, uint32_t>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    uint32_t current_node_dist = pq.top().first;
    uint32_t current_node_id = pq.top().second;
    pq.pop();

    if (current_node_dist > distances[current_node_id]) {
      continue;
    }

    // Check the neighbors of the current node
    uint32_t start = node_offsets[current_node_id];
    uint32_t end = node_offsets[current_node_id + 1];
    for (uint32_t i = start; i < end; ++i) {
      const Edge &neighbor = edges[i];

      if (distances[current_node_id] + neighbor.weight <
          distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] =
            distances[current_node_id] + neighbor.weight;
        pq.push({distances[neighbor.trg_id], neighbor.trg_id});
      }
    }
  }

  return distances;
}

uint32_t Dijkstra::src_to_trg(uint32_t src, uint32_t trg,
                              uint32_t &nodes_checked) {
  using std::pair;
  using std::vector;

  vector<uint32_t> distances(node_offsets.size() - 1, UINT32_MAX);
  distances[src] = 0;
  std::priority_queue<pair<uint32_t, uint32_t>,
                      vector<pair<uint32_t, uint32_t>>,
                      std::greater<pair<uint32_t, uint32_t>>>
      pq;
  pq.push({0, src});

  while (!pq.empty()) {
    // Get the closest node to the source
    uint32_t current_node_dist = pq.top().first;
    uint32_t current_node_id = pq.top().second;
    pq.pop();

    // Target found. Early return
    if (current_node_id == trg) {
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
    uint32_t start = node_offsets[current_node_id];
    uint32_t end = node_offsets[current_node_id + 1];
    for (size_t i = start; i < end; i++) {
      const Edge &neighbor = edges[i];
      const uint32_t new_cost = distances[current_node_id] + neighbor.weight;

      if (new_cost < distances[neighbor.trg_id]) {
        distances[neighbor.trg_id] = new_cost;
        pq.push({new_cost, neighbor.trg_id});
      }
    }
  }

  // Target node was not found
  return UINT32_MAX;
}
