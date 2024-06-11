#pragma once

#include "dijkstra.h"
#include "edge.h"

#include <vector>

class CLI {
public:
  void run(std::vector<int> &node_offsets,
           std::vector<Edge> &edges);

private:
  void print_menu();
  void handle_one_to_all(Dijkstra &dijkstra);
  void handle_src_to_trg(Dijkstra &dijkstra);
};
