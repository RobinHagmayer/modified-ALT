#pragma once

#include "edge.h"

#include <string>
#include <vector>

class Graph_parser {
public:
  void parse(const std::string &file_path, std::vector<int> &node_offsets,
             std::vector<Edge> &edges, bool reverse);

private:
  void parse_edges(const std::string &line, int &curr_node, int &edge_counter,
                   std::vector<int> &node_offsets, std::vector<Edge> &edges,
                   bool reverse);
};
