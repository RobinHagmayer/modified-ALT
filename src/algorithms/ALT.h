#pragma once

#include "graph.h"

#include <cstdint>
#include <vector>

class ALT {
public:
  const std::vector<uint32_t> &node_offsets;
  const std::vector<Edge> &edges;
  const std::vector<std::vector<uint32_t>> &to_lm_distances;

  ALT(const Graph &graph,
      const std::vector<std::vector<uint32_t>> &to_lm_distances)
      : node_offsets(graph.node_offsets), edges(graph.edges),
        to_lm_distances(to_lm_distances){};

  uint32_t src_to_trg(uint32_t src_id, uint32_t trg_id, uint32_t &nodes_checked,
                      size_t lm_count);

  int32_t h(uint32_t node_id, uint32_t trg_id, size_t lm_count);
};
