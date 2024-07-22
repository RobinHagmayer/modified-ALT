#pragma once

#include <ostream>
#include <vector>

struct Edge {
  uint32_t src_id;
  uint32_t trg_id;
  uint32_t weight;

  /*Edge(int s, int t, int w) : src_id(s), trg_id(t), weight(w) {}*/

  /*Edge() : src_id(0), trg_id(0), weight(0) {}*/

  friend std::ostream &operator<<(std::ostream &os, const Edge &edge);
};

struct Graph {
  std::vector<uint32_t> node_offsets;
  std::vector<Edge> edges;
};
