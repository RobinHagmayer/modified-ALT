#pragma once

#include "edge.h"

#include <optional>
#include <string>
#include <vector>

struct DijkstraAllResult {
  bool success;
  std::optional<std::string> error_message;
  std::optional<std::vector<int>> distances;
};

struct DijkstraSingleResult {
  bool success;
  std::optional<std::string> error_message;
  std::optional<int> cost;
};

class Dijkstra {
public:
  const std::vector<int> &node_offsets_;
  const std::vector<Edge> &edges_;

public:
  Dijkstra(const std::vector<int> &no, const std::vector<Edge> &e)
      : node_offsets_(no), edges_(e){};
  DijkstraAllResult src_to_all(int src);
  DijkstraSingleResult src_to_trg(int src, int trg);
};
