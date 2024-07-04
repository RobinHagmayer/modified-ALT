#pragma once

#include "edge.h"

#include <string>
#include <unordered_map>
#include <vector>

class Benchmark {
public:
  void run(const int &argc, const char *const *argv);

private:
  static const std::string graph_dir;
  static const std::string bench_dir;

  void handle_cla(const int &argc, const char *const *argv,
                  bool &create_binary_graph, bool &create_binary_preprocessing,
                  bool &use_binary, std::string &graph_path,
                  std::string &graph_path_rev, std::string &bench_path);
  void print_help();
  void parse_bench_file(const std::string &bench_path,
                        std::vector<std::pair<int, int>> &route_requests);
  void bench_dijkstra(const std::vector<int> &node_offsets,
                      const std::vector<Edge> &edges,
                      const std::vector<std::pair<int, int>> &route_requests);
  void bench_alt(
      const std::vector<int> &node_offsets, const std::vector<Edge> &edges,
      const std::vector<int> &node_offsets_rev,
      const std::vector<Edge> &edges_rev,
      const std::vector<std::pair<int, int>> &route_requests,
      const std::unordered_map<int, std::vector<int>> &landmark_distances);
  void debug(const std::string &graph_path);
};
