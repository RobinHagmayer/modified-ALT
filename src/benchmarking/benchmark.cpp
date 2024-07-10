#include "benchmark.h"
#include "ALT.h"
#include "benchmark_file_generator.h"
#include "dijkstra.h"
#include "farthest.h"
#include "graph_parser.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const std::string Benchmark::graph_dir = std::string(GRAPH_DIR);
const std::string Benchmark::bench_dir = std::string(BENCH_DIR);

void Benchmark::run(const int &argc, const char *const *argv) {
  /*debug(argv[1]);*/
  /*return;*/

  std::string graph;
  std::string graph_path;
  std::string graph_path_rev;
  std::string bench_path;

  bool use_random_bench = true;
  bool bench_dijkstra_bool = false;

  Graph_parser gp;
  std::vector<int> node_offsets;
  std::vector<Edge> edges;
  std::vector<int> node_offsets_rev;
  std::vector<Edge> edges_rev;
  std::vector<std::vector<int>> landmark_distances_forward;
  std::vector<std::vector<int>> landmark_distances_reverse;

  if (argc <= 1) {
    print_help();
    exit(EXIT_SUCCESS);
  } else if (argc == 2) {
    std::string option = argv[1];
    std::string help_option = "-h";

    if (option == help_option) {
      print_help();
      exit(EXIT_SUCCESS);
    }

    // Default usage.
    // Uses binary graph and preprocessing files.
    // Uses random benchmarks.
    graph = argv[1];
    graph_path = graph_dir + argv[1];
    graph_path_rev = graph_dir + argv[1] + "_rev.bin";
    bench_path = bench_dir + "random_" + argv[1] + ".que";
  } else if (argc == 3) {
    std::string option = argv[1];
    std::string b_option = "-b";
    std::string p_option = "-p";
    std::string d_option = "-d";

    if (option == b_option) {
      // Create binary graph files
      graph_path = graph_dir + argv[2];
      use_random_bench = false;

      gp.parse(graph_path + ".fmi", node_offsets, edges, false);
      gp.serialize(graph_path + ".bin", node_offsets, edges);

      gp.parse(graph_path + ".fmi", node_offsets_rev, edges_rev, true);
      gp.serialize(graph_path + "_rev.bin", node_offsets_rev, edges_rev);

      std::cout << "Binary files created successfully!\n";
      exit(EXIT_SUCCESS);
    } else if (option == p_option) {
      // Create binary preprocessing files
      graph_path = graph_dir + argv[2];
      use_random_bench = false;

      gp.deserialize(graph_path + ".bin", node_offsets, edges);
      gp.deserialize(graph_path + "_rev.bin", node_offsets_rev, edges_rev);

      Dijkstra dijkstra_forward(node_offsets, edges);
      Dijkstra dijkstra_reverse(node_offsets_rev, edges_rev);
      Farthest::preprocess(16, dijkstra_forward, dijkstra_reverse,
                           landmark_distances_forward,
                           landmark_distances_reverse);

      Farthest::serialize(graph_path + "_pre.bin", landmark_distances_reverse);
      exit(EXIT_SUCCESS);
    } else if (option == d_option) {
      // Bench Dijkstra's algorithm and ALT
      bench_dijkstra_bool = true;

      graph = argv[2];
      graph_path = graph_dir + argv[2];
      graph_path_rev = graph_dir + argv[2] + "_rev.bin";
      bench_path = bench_dir + "random_" + argv[2] + ".que";
    } else {
      print_help();
      exit(EXIT_SUCCESS);
    }
  } else if (argc == 4) {
    // Use specified bench file
    /*bench_dijkstra_bool = true;*/
    use_random_bench = false;
    graph_path = graph_dir + argv[3];
    graph_path_rev = graph_dir + argv[3] + "_rev.bin";
    bench_path = bench_dir + argv[2] + ".que";
  } else {
    print_help();
    exit(EXIT_SUCCESS);
  }

  if (use_random_bench) {
    gp.deserialize(graph_path + ".bin", node_offsets, edges);
    Dijkstra dijkstra(node_offsets, edges);

    Benchmark_file_generator bfg(dijkstra);
    bfg.create_files(100, graph);
  } else {
    gp.deserialize(graph_path + ".bin", node_offsets, edges);
  }

  std::vector<std::pair<int, int>> route_requests;
  parse_bench_file(bench_path, route_requests);

  if (bench_dijkstra_bool) {
    bench_dijkstra(node_offsets, edges, route_requests);
    std::cout << "\n";
  }

  std::vector<std::vector<int>> landmark_distances;
  Farthest::deserialize(graph_path + "_pre.bin", landmark_distances);
  bench_alt(node_offsets, edges, node_offsets_rev, edges_rev, route_requests,
            landmark_distances);
}

void Benchmark::print_help() {
  using std::cout;
  cout << "Usage: modified-ALT-bench OPTIONS GRAPH" << "\n\n";
  cout << "OPTIONS:\n";
  cout << "  -h: Displays this help message.\n";
  cout << "  -d: Benchmark Dijkstra as well.\n";
  cout << "  -p: Create the binary files for faster preprocessing.\n";
  cout << "  -b: Create the binary files from the given GRAPH.\n\n";
  cout << "GRAPH:\n";
  cout << "  The graph you want to use. The graph file should be "
          "located at 'data/graphs/' and use the '.bin' ending.\n";
  /*cout << "BENCH:\n";*/
  /*cout << "  The benchmark you want to use. The benchmark file should be "*/
  /*        "located at 'data/benchs/' and use the '.que' ending.\n\n";*/
}

void Benchmark::parse_bench_file(
    const std::string &bench_path,
    std::vector<std::pair<int, int>> &route_requests) {
  std::string line;
  int src_id;
  int trg_id;

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
}

void Benchmark::bench_dijkstra(
    const std::vector<int> &node_offsets, const std::vector<Edge> &edges,
    const std::vector<std::pair<int, int>> &route_requests) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::cout;
  using std::endl;

  cout << "----------------------------------------" << endl;
  cout << "Starting Dijkstra benchmark." << endl;
  cout << "----------------------------------------" << endl;

  long dijkstra_time = 0;
  std::vector<int> path_costs_dijkstra;
  Dijkstra dijsktra(node_offsets, edges);

  int search_space = 0;

  for (int i = 0; i < route_requests.size(); i++) {
    int src_id = route_requests[i].first;
    int trg_id = route_requests[i].second;
    int nodes_checked = 0;

    auto start_dijsktra = Clock::now();
    int path_cost_dijkstra = dijsktra.src_to_trg(src_id, trg_id, nodes_checked);
    auto end_dijkstra = Clock::now();
    auto duration_dijkstra =
        std::chrono::duration_cast<Milliseconds>(end_dijkstra - start_dijsktra);

    cout << "\r" << i + 1 << "%" << std::flush;

    search_space += nodes_checked;

    dijkstra_time += duration_dijkstra.count();
    path_costs_dijkstra.push_back(path_cost_dijkstra);
  }
  cout << endl;

  cout << "----------------------------------------" << endl;
  cout << "Dijkstra average execution time: "
       << dijkstra_time / route_requests.size() << "ms." << endl;
  cout << "----------------------------------------" << endl;

  cout << "Dijkstra average search space: "
       << search_space / route_requests.size() << endl;
  cout << "----------------------------------------" << endl;

  std::ofstream route_costs_dijkstra_file(std::string(BENCH_DIR) +
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

void Benchmark::bench_alt(
    const std::vector<int> &node_offsets, const std::vector<Edge> &edges,
    const std::vector<int> &node_offsets_rev,
    const std::vector<Edge> &edges_rev,
    const std::vector<std::pair<int, int>> &route_requests,
    std::vector<std::vector<int>> &landmark_distances) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::cout;
  using std::endl;

  cout << "----------------------------------------" << endl;
  cout << "Starting ALT benchmark." << endl;
  cout << "----------------------------------------" << endl;

  long alt_time = 0;
  Dijkstra dijkstra_forward(node_offsets, edges);
  Dijkstra dijkstra_rev(node_offsets_rev, edges_rev);
  std::vector<int> path_costs_alt;
  std::vector<std::vector<int>> landmark_distances_forward;
  std::vector<std::vector<int>> landmark_distances_reverse;
  int search_space = 0;

  /*Random_landmarks::preprocess(16, dijkstra_forward, dijkstra_rev,*/
  /*                             landmark_distances_forward,*/
  /*                             landmark_distances_reverse);*/
  /*Farthest::preprocess(16, dijkstra_forward, dijkstra_rev,*/
  /*                     landmark_distances_reverse);*/

  ALT alt(node_offsets, edges);

  for (int i = 0; i < route_requests.size(); i++) {
    int src_id = route_requests[i].first;
    int trg_id = route_requests[i].second;
    int nodes_checked = 0;

    sort_landmark_distances(src_id, trg_id, landmark_distances);

    auto start_alt = Clock::now();
    int path_cost_alt =
        alt.src_to_trg(src_id, trg_id, nodes_checked, landmark_distances);
    auto end_alt = Clock::now();
    auto duration_alt =
        std::chrono::duration_cast<Milliseconds>(end_alt - start_alt);

    cout << "\r" << i + 1 << "%" << std::flush;

    search_space += nodes_checked;

    alt_time += duration_alt.count();
    path_costs_alt.push_back(path_cost_alt);
  }
  cout << endl;

  cout << "----------------------------------------" << endl;
  cout << "ALT average execution time: " << alt_time / route_requests.size()
       << "ms." << endl;
  cout << "----------------------------------------" << endl;

  cout << "ALT average search space: " << search_space / route_requests.size()
       << endl;
  cout << "----------------------------------------" << endl;

  std::ofstream route_costs_alt_file(std::string(BENCH_DIR) +
                                     "route_costs_alt.txt");
  if (!route_costs_alt_file) {
    std::cerr << "Error opening file." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs_alt) {
    route_costs_alt_file << cost << endl;
  }

  route_costs_alt_file.close();
}

void Benchmark::sort_landmark_distances(
    const int &src_id, const int &trg_id,
    std::vector<std::vector<int>> &landmark_distances) {
  std::sort(landmark_distances.begin(), landmark_distances.end(),
            [src_id, trg_id](auto &a, auto &b) -> bool {
              if (a[src_id] == INT_MAX || a[trg_id] == INT_MAX ||
                  b[src_id] == INT_MAX || b[trg_id] == INT_MAX) {
                return INT_MIN;
              }
              return (a[src_id] - a[trg_id]) > (b[src_id] - b[trg_id]);
            });
}

void Benchmark::debug(const std::string &graph) {
  Graph_parser gp;
  std::vector<int> node_offsets;
  std::vector<Edge> edges;
  /*std::vector<int> node_offsets_rev;*/
  /*std::vector<Edge> edges_rev;*/

  gp.deserialize(std::string(GRAPH_DIR) + graph + ".bin", node_offsets, edges);
  /*gp.deserialize(std::string(GRAPH_DIR) + "MV_rev.bin", node_offsets_rev,*/
  /*               edges_rev);*/

  Dijkstra dijkstra_forward(node_offsets, edges);
  /*Dijkstra dijkstra_reverse(node_offsets_rev, edges_rev);*/

  Benchmark_file_generator bfg(dijkstra_forward);
  bfg.create_files(100, graph);
}
