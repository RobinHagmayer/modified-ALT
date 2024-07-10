#pragma once

#include "edge.h"

#include <vector>

class ALT {
public:
  const std::vector<int> &node_offsets_;
  const std::vector<Edge> &edges_;

  ALT(const std::vector<int> &no, const std::vector<Edge> &e)
      : node_offsets_(no), edges_(e){};

  int src_to_trg(int src_id, int trg_id, int &nodes_checked,
                 const std::vector<std::vector<int>> &landmark_distances);
  int h(int node_id, int trg_id,
        const std::vector<std::vector<int>> &landmark_distances);
};
