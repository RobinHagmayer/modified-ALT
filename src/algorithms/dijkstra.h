#pragma once

#include "edge.h"

#include <vector>

class Dijkstra {
public:
  const std::vector<int> &node_offsets_;
  const std::vector<Edge> &edges_;

  Dijkstra(const std::vector<int> &no, const std::vector<Edge> &e)
      : node_offsets_(no), edges_(e){};
  void src_to_all(int src);
  int src_to_trg(int src, int trg);
};
