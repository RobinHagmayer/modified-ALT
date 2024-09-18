#pragma once

#include "graph.h"
#include "new.h"

#include <climits>
#include <cstdint>
#include <string>
#include <vector>

/**
 * Stores the pair of point-to-point shortest path queries
 * (source_id, target_id).
 */
typedef std::vector<std::pair<uint32_t, uint32_t>> route_requests_vec;

const std::string GRAPH_DIR = "./data/graphs/";
const std::string BENCH_DIR = "./data/benchs/";

/**
 * @brief Represents a benchmark.
 *
 * The Benchmark class provides functionality for running benchmarks and
 * creating benchmarks for point-to-point shortest path queries.
 * The algorithms which can be benchmarked are:
 *   - Dijkstra
 *   - Default ALT
 *   - Modified ALT
 */
class Benchmark {
public:
  /**
   * Run the benchmark.
   * @param argc The number of command line arguments.
   * @param argv The array of command line arguments.
   */
  void run(int argc, char *argv[]);

  void create_binary_graph(const std::string &graph_path);

  void preprocess(const std::string &graph_path);

  void create_random_bench(const std::string &graph_path,
                           const std::string &graph_option,
                           uint32_t route_request_count);

  route_requests_vec parse_bench_file(const std::string &bench_path);

  void bench_dijkstra(const Graph &graph,
                      const route_requests_vec &route_requests);

  void bench_new_alt(size_t lm_count, const Graph &graph,
                     const Graph &graph_reverse,
                     const route_requests_vec &route_requests,
                     const New_lm_data &lm_data);

  void bench_alt(size_t lm_count, const Graph &graph,
                 const Graph &graph_reverse,
                 const route_requests_vec &route_requests,
                 std::vector<std::vector<uint32_t>> &to_lm_distances);

  void sort_landmark_distances(
      const uint32_t &src_id, const uint32_t &trg_id,
      std::vector<std::vector<uint32_t>> &landmark_distances);
};
