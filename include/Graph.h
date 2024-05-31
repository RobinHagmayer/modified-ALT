#pragma once

#include <string>
#include <vector>

struct edge {
  int src, trg, weight;
};

class Graph {
public:
  int number_of_nodes_;
  int number_of_edges_;

private:
  std::vector<int> offset_for_node_;
  std::vector<edge> edges_;

public:
  void parse_graph_file(const std::string &file_path);
  std::vector<int> get_offset_for_node();
  std::vector<edge> get_edges();

private:
  void parse_edges(const std::string &line);
};
