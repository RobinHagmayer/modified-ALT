#pragma once

#include "Edge.h"

#include <string>
#include <vector>

struct GraphParserResult {
  int number_of_nodes;
  int number_of_edges;

  std::vector<int> node_offsets;
  std::vector<Edge> edges;

  GraphParserResult(int non, int noe, std::vector<int> node_offsets,
                    std::vector<Edge> edges)
      : number_of_nodes(non), number_of_edges(noe), node_offsets(node_offsets),
        edges(edges) {}
};

class GraphParser {
public:
  GraphParserResult parse(const std::string &file_path, bool verbose = false);
  void parse_edges(const std::string &line, int &curr_node, int &edge_counter,
                   std::vector<int> &node_offsets, std::vector<Edge> &edges);
};
