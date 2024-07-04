#pragma once

#include "edge.h"

#include <string>
#include <vector>

class Graph_parser {
public:
  void parse(const std::string &file_path, std::vector<int> &node_offsets,
             std::vector<Edge> &edges, bool reverse);

  void serialize(const std::string &file_path,
                 const std::vector<int> &node_offsets,
                 const std::vector<Edge> &edges);

  void deserialize(const std::string &file_path, std::vector<int> &node_offsets,
                   std::vector<Edge> &edges);

private:
  void parse_edges(const std::string &line, std::vector<Edge> &edges,
                   bool reverse);

  void create_offset_array(const std::vector<Edge> &edges,
                           std::vector<int> &node_offsets);
};
