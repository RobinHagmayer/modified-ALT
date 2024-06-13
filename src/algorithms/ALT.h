#pragma once

#include "edge.h"

#include <unordered_map>
#include <vector>

class ALT {
public:
  const std::vector<int> &node_offsets_;
  const std::vector<Edge> &edges_;
  const std::unordered_map<int, std::vector<int>> &landmark_distances_;

  ALT(const std::vector<int> &no, const std::vector<Edge> &e,
      const std::unordered_map<int, std::vector<int>> &ld)
      : node_offsets_(no), edges_(e), landmark_distances_(ld){};

  int src_to_trg(int src_id, int trg_id);
  int h(int node_id, int trg_id);
};
