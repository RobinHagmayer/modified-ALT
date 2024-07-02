#pragma once

#include <string>
#include <vector>

class Benchmark {
public:
  struct Result {
    bool ok;
    std::string message;
  };

  void run(const int &argc, const char *const *argv);

private:
  static const std::string graph_dir;
  static const std::string bench_dir;

  Result validate_cla(const int &argc, const char *const *argv);
  Result parse_bench_file(const std::string &bench_path,
                          std::vector<std::pair<int, int>> &route_requests);
  void bench_dijkstra(const std::string &graph_path,
                      const std::vector<std::pair<int, int>> &route_requests);
  void bench_alt(const std::string &graph_path,
                 const std::vector<std::pair<int, int>> &route_requests);
  void debug(const std::string &graph_path);
};
