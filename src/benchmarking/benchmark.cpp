#include "benchmark.h"
#include "ALT.h"
#include "argparse.hpp"
#include "benchmark_file_generator.h"
#include "dijkstra.h"
#include "farthest.h"
#include "graph.h"
#include "graph_parser.h"
#include "new.h"
#include "new_ALT.h"

#include <chrono>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <vector>

struct BenchmarkArgs : public argparse::Args {
  std::string &graph_filename =
      arg("graph", "The graph which should be used for the benchmark");

  int &algorithm =
      kwarg("a,algorithm", "The algorithm which should be used\n\t\t\t- 1: "
                           "Dijkstra's Algorithm\n\t\t\t- 2: ALT\n\t\t\t- 3: "
                           "Modified ALT\n\t\t\t- 0: All of the above")
          .set_default(0);

  std::string &benchmark =
      kwarg("B,benchmark", "The benchmark which should be used")
          .set_default("random");

  std::optional<uint32_t> &random_bench =
      kwarg("r,random", "Create a random benchmark for the specified graph "
                        "with the given number of route requests");

  bool &binary = flag("b,binary", "Create the necessary binary graph files");
  bool &preprocess =
      flag("p,preprocess",
           "Create the necessary preprocess files for the ALT algorithms");
  bool &verbose = flag("v,verbose", "More detailed output to the user");
};

void Benchmark::run(int argc, char *argv[]) {
  auto args = argparse::parse<BenchmarkArgs>(argc, argv);

  if (args.binary) {
    create_binary_graph(GRAPH_DIR + args.graph_filename);
  }

  if (args.preprocess) {
    preprocess(GRAPH_DIR + args.graph_filename);
  }

  if (args.random_bench.has_value()) {
    create_random_bench(GRAPH_DIR + args.graph_filename, args.graph_filename,
                        args.random_bench.value());
  }

  std::string bench_option = args.benchmark;

  if (args.benchmark == "random") {
    bench_option = args.benchmark + "_" + args.graph_filename;
  }

  route_requests_vec route_requests =
      parse_bench_file(BENCH_DIR + bench_option + ".que");

  Graph_parser gp;
  Graph graph = gp.deserialize(GRAPH_DIR + args.graph_filename + ".bin");

  if (args.algorithm == 1) { // Dijkstra
    bench_dijkstra(graph, route_requests);
    exit(EXIT_SUCCESS);
  } else if (args.algorithm == 2) { // ALT
    Graph graph_reverse =
        gp.deserialize(GRAPH_DIR + args.graph_filename + "_rev.bin");

    std::vector<std::vector<uint32_t>> from_lm_distances;
    std::vector<std::vector<uint32_t>> to_lm_distances;
    Farthest::deserialize(GRAPH_DIR + args.graph_filename + "_pre.bin",
                          from_lm_distances, to_lm_distances);

    bench_alt(3, graph, graph_reverse, route_requests, to_lm_distances);
    exit(EXIT_SUCCESS);
  } else if (args.algorithm == 3) { // Modified ALT
    Graph graph_reverse =
        gp.deserialize(GRAPH_DIR + args.graph_filename + "_rev.bin");

    New_lm_data lm_data =
        New::deserialize(GRAPH_DIR + args.graph_filename + "_new_pre.bin");

    uint32_t lm_count = std::sqrt(graph.node_offsets.size() - 1);
    bench_new_alt(lm_count, graph, graph_reverse, route_requests, lm_data);
    /*bench_new_alt(256, graph, graph_reverse, route_requests, lm_data);*/
    exit(EXIT_SUCCESS);
  } else if (args.algorithm == 0) { // All algorithms
    bench_dijkstra(graph, route_requests);
    std::cout << "\n";

    Graph graph_reverse =
        gp.deserialize(GRAPH_DIR + args.graph_filename + "_rev.bin");

    std::vector<std::vector<uint32_t>> from_lm_distances;
    std::vector<std::vector<uint32_t>> to_lm_distances;
    Farthest::deserialize(GRAPH_DIR + args.graph_filename + "_pre.bin",
                          from_lm_distances, to_lm_distances);

    bench_alt(3, graph, graph_reverse, route_requests, to_lm_distances);
    std::cout << "\n";

    New_lm_data lm_data =
        New::deserialize(GRAPH_DIR + args.graph_filename + "_new_pre.bin");

    uint32_t lm_count = std::sqrt(graph.node_offsets.size() - 1);
    bench_new_alt(lm_count, graph, graph_reverse, route_requests, lm_data);
    exit(EXIT_SUCCESS);
  }

  std::cerr << "This state of the program is not possible" << std::endl;

  exit(EXIT_FAILURE);
}

void Benchmark::create_binary_graph(const std::string &graph_path) {
  std::cout << "Starting creation of binary graph files..." << std::endl;
  Graph_parser gp;

  Graph graph = gp.parse(graph_path + ".fmi", false);
  Graph graph_reverse = gp.parse(graph_path + ".fmi", true);

  gp.serialize(graph_path + ".bin", graph);
  gp.serialize(graph_path + "_rev.bin", graph_reverse);

  std::cout << "Binary files created successfully!" << std::endl;
  exit(EXIT_SUCCESS);
}

void Benchmark::preprocess(const std::string &graph_path) {
  Graph_parser gp;

  Graph graph = gp.deserialize(graph_path + ".bin");
  Graph graph_reverse = gp.deserialize(graph_path + "_rev.bin");

  Farthest_lm_data lm_data = Farthest::preprocess(16, graph, graph_reverse);

  Farthest::serialize(graph_path + "_pre.bin", lm_data.from_lm_distances,
                      lm_data.to_lm_distances);

  uint32_t lm_count = std::sqrt(graph.node_offsets.size() - 1);
  New_lm_data new_lm_data = New::preprocess(lm_count, graph, graph_reverse);
  /*New_lm_data new_lm_data = New::preprocess(256, graph, graph_reverse);*/

  New::serialize(graph_path + "_new_pre.bin", new_lm_data);

  exit(EXIT_SUCCESS);
}

void Benchmark::create_random_bench(const std::string &graph_path,
                                    const std::string &graph_option,
                                    uint32_t route_request_count) {
  std::cout << "Starting generating random benchmark files" << "\n";
  Graph_parser gp;
  Graph graph = gp.deserialize(graph_path + ".bin");

  Dijkstra dijkstra(graph);

  Benchmark_file_generator bfg(dijkstra);
  bfg.create_files(route_request_count, graph_option);

  std::cout << "Finished generating random benchmark files" << "\n";
  exit(EXIT_SUCCESS);
}

route_requests_vec Benchmark::parse_bench_file(const std::string &bench_path) {
  route_requests_vec route_requests;
  std::string line;
  uint32_t src_id;
  uint32_t trg_id;

  std::ifstream bench_file(bench_path);
  if (!bench_file.is_open()) {
    std::cerr << "Error opening the file: " + bench_path + ".\n";
    exit(EXIT_FAILURE);
  }

  while (std::getline(bench_file, line)) {
    std::istringstream line_stream(line);
    if (!(line_stream >> src_id >> trg_id)) {
      std::cerr << "Error reading the line: " + line + ".\n";
      exit(EXIT_FAILURE);
    }
    route_requests.push_back({src_id, trg_id});
  }

  bench_file.close();

  return route_requests;
}

void Benchmark::bench_dijkstra(const Graph &graph,
                               const route_requests_vec &route_requests) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::cout;
  using std::endl;

  cout << "----------------------------------------" << endl;
  cout << "Starting Dijkstra benchmark." << endl;
  cout << "----------------------------------------" << endl;

  long dijkstra_time = 0;
  uint32_t search_space = 0;
  std::vector<uint32_t> path_costs_dijkstra;
  Dijkstra dijsktra(graph);

  for (size_t i = 0; i < route_requests.size(); ++i) {
    uint32_t src_id = route_requests[i].first;
    uint32_t trg_id = route_requests[i].second;
    uint32_t nodes_checked = 0;

    auto start_dijsktra = Clock::now();
    uint32_t cost = dijsktra.src_to_trg(src_id, trg_id, nodes_checked);
    auto end_dijkstra = Clock::now();
    auto duration_dijkstra =
        std::chrono::duration_cast<Milliseconds>(end_dijkstra - start_dijsktra);

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;

    search_space += nodes_checked;

    dijkstra_time += duration_dijkstra.count();
    path_costs_dijkstra.push_back(cost);
  }
  cout << endl;

  cout << "----------------------------------------" << endl;
  cout << "Dijkstra average execution time: "
       << dijkstra_time / route_requests.size() << "ms" << endl;
  cout << "----------------------------------------" << endl;

  cout << "Dijkstra average search space: "
       << search_space / route_requests.size() << endl;
  cout << "----------------------------------------" << endl;

  std::ofstream route_costs_dijkstra_file(BENCH_DIR +
                                          "route_costs_dijkstra.txt");
  if (!route_costs_dijkstra_file) {
    std::cerr << "Error opening file." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs_dijkstra) {
    route_costs_dijkstra_file << cost << endl;
  }

  route_costs_dijkstra_file.close();
}

void Benchmark::bench_alt(size_t lm_count, const Graph &graph,
                          const Graph &graph_reverse,
                          const route_requests_vec &route_requests,
                          std::vector<std::vector<uint32_t>> &to_lm_distances) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Starting ALT benchmark." << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  long alt_time = 0;
  uint32_t search_space = 0;
  std::vector<uint32_t> path_costs_alt;
  /*ALT alt(graph, lm_data.to_lm_distances);*/
  ALT alt(graph, to_lm_distances);

  /*for (size_t i = 0; i < 1; ++i) {*/
  for (size_t i = 0; i < route_requests.size(); ++i) {
    uint32_t src_id = route_requests[i].first;
    uint32_t trg_id = route_requests[i].second;
    uint32_t nodes_checked = 0;

    sort_landmark_distances(src_id, trg_id, to_lm_distances);

    auto start_alt = Clock::now();
    uint32_t cost = alt.src_to_trg(src_id, trg_id, nodes_checked, lm_count);
    auto end_alt = Clock::now();
    auto duration_alt =
        std::chrono::duration_cast<Milliseconds>(end_alt - start_alt);

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;

    search_space += nodes_checked;

    alt_time += duration_alt.count();
    path_costs_alt.push_back(cost);
  }

  std::cout << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "ALT average execution time: "
            << alt_time / route_requests.size() << "ms" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "ALT average search space: "
            << search_space / route_requests.size() << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  std::ofstream route_costs_alt_stream(BENCH_DIR + "route_costs_alt.txt");
  if (!route_costs_alt_stream) {
    std::cerr << "Error opening file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs_alt) {
    route_costs_alt_stream << cost << std::endl;
  }

  route_costs_alt_stream.close();
}
void Benchmark::bench_new_alt(size_t lm_count, const Graph &graph,
                              const Graph &graph_reverse,
                              const route_requests_vec &route_requests,
                              const New_lm_data &lm_data) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Starting new ALT benchmark." << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  long alt_time = 0;
  uint32_t search_space = 0;
  std::vector<uint32_t> path_costs_alt;

  New_ALT new_alt(graph, lm_data);

  /*for (size_t i = 0; i < 1; ++i) {*/
  for (size_t i = 0; i < route_requests.size(); ++i) {
    uint32_t src_id = route_requests[i].first;
    uint32_t trg_id = route_requests[i].second;
    uint32_t nodes_checked = 0;

    auto start_alt = Clock::now();
    uint32_t cost = new_alt.src_to_trg(src_id, trg_id, nodes_checked);
    auto end_alt = Clock::now();
    auto duration_alt =
        std::chrono::duration_cast<Milliseconds>(end_alt - start_alt);

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;

    search_space += nodes_checked;

    alt_time += duration_alt.count();
    path_costs_alt.push_back(cost);
  }

  std::cout << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "New ALT average execution time: "
            << alt_time / route_requests.size() << "ms" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "New ALT average search space: "
            << search_space / route_requests.size() << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  std::ofstream route_costs_alt_stream(BENCH_DIR + "route_costs_new_alt.txt");
  if (!route_costs_alt_stream) {
    std::cerr << "Error opening file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs_alt) {
    route_costs_alt_stream << cost << std::endl;
  }

  route_costs_alt_stream.close();
}

void Benchmark::sort_landmark_distances(
    const uint32_t &src_id, const uint32_t &trg_id,
    std::vector<std::vector<uint32_t>> &landmark_distances) {
  std::sort(landmark_distances.begin(), landmark_distances.end(),
            [src_id, trg_id](auto &a, auto &b) {
              if (a[src_id] == UINT32_MAX || a[trg_id] == UINT32_MAX) {
                return true;
              } else if (b[src_id] == UINT32_MAX || b[trg_id] == UINT32_MAX) {
                return false;
              }

              return (static_cast<int32_t>(a[src_id]) -
                      static_cast<int32_t>(a[trg_id])) >
                     (static_cast<int32_t>(b[src_id]) -
                      static_cast<int32_t>(b[trg_id]));
            });
}
