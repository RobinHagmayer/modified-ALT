#pragma once

#include "edge.h"

#include <string>
#include <vector>

class App {
public:
  struct Result {
    bool ok;
    std::string message;
  };

  void run(const int &argc, const char *const *argv);

private:
  static const std::string graph_dir;

  Result validate_cla(const int &argc, const char *const *argv);
  void read_graph(const std::string &file_path, std::vector<int> &node_offsets,
                  std::vector<Edge> &edges);
};
