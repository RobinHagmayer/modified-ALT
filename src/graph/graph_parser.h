#pragma once

#include "graph.h"

#include <string>
#include <vector>

class Graph_parser {
public:
  Graph parse(const std::string &file_path, bool reverse);

  void parse_edges(const std::string &line, std::vector<Edge> &edges,
                   bool reverse);

  void create_offset_array(const std::vector<Edge> &edges,
                           std::vector<uint32_t> &node_offsets);

  void serialize(const std::string &file_path, const Graph &graph);

  Graph deserialize(const std::string &file_path);
};
