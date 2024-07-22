#pragma once

#include "graph.h"

#include <cstdint>
#include <vector>

class Dijkstra {
public:
  const std::vector<uint32_t> &node_offsets;
  const std::vector<Edge> &edges;

  Dijkstra(const Graph &graph)
      : node_offsets(graph.node_offsets), edges(graph.edges){};

  std::vector<uint32_t> src_to_all(uint32_t src);

  void src_to_all(uint32_t src, std::vector<uint32_t> &distances);

  uint32_t src_to_trg(uint32_t src, uint32_t trg, uint32_t &nodes_checked);
};
