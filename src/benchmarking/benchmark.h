#pragma once

#include "edge.h"

#include <climits>
#include <string>
#include <vector>

class Benchmark {
public:
  void run(const int &argc, const char *const *argv);

private:
  static const std::string graph_dir;
  static const std::string bench_dir;

  void print_help();
  void parse_bench_file(const std::string &bench_path,
                        std::vector<std::pair<int, int>> &route_requests);
  void bench_dijkstra(const std::vector<int> &node_offsets,
                      const std::vector<Edge> &edges,
                      const std::vector<std::pair<int, int>> &route_requests);
  void bench_alt(const std::vector<int> &node_offsets,
                 const std::vector<Edge> &edges,
                 const std::vector<int> &node_offsets_rev,
                 const std::vector<Edge> &edges_rev,
                 const std::vector<std::pair<int, int>> &route_requests,
                 std::vector<std::vector<int>> &landmark_distances);
  void debug(const std::string &graph_path);
  void
  sort_landmark_distances(const int &src_id, const int &trg_id,
                          std::vector<std::vector<int>> &landmark_distances);
};
