#pragma once

#include "Edge.h"

#include <optional>
#include <string>
#include <vector>

struct GraphParserResult {
  bool success;
  std::optional<std::string> error_message;

  std::optional<int> number_of_nodes;
  std::optional<int> number_of_edges;
};

class GraphParser {
public:
  GraphParserResult parse(const std::string &file_path,
                          std::vector<int> &node_offsets,
                          std::vector<Edge> &edges, bool verbose = false);
  void parse_edges(const std::string &line, int &curr_node, int &edge_counter,
                   std::vector<int> &node_offsets, std::vector<Edge> &edges);
};
