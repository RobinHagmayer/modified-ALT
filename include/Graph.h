#pragma once

#include <string>
#include <vector>
#include <iostream>

struct Edge {
  int src_id, trg_id, weight;
};

std::ostream& operator<<(std::ostream& os, const Edge& edge);

class Graph {
public:
  int number_of_nodes_;
  int number_of_edges_;

private:
  std::vector<int> offset_for_node_;
  std::vector<Edge> edges_;

public:
  void parse_graph_file(const std::string &file_path, bool verbose = false);
  std::vector<int> get_offset_for_node();
  std::vector<Edge> get_edges();

private:
  void parse_edges(const std::string &line, int &curr_node, int &edge_counter);
};
