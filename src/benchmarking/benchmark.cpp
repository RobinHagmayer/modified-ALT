#include "benchmark.h"
#include "dijkstra.h"
#include "graph_parser.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const std::string Benchmark::graph_dir = std::string(GRAPH_DIR);
const std::string Benchmark::bench_dir = std::string(BENCH_DIR);

void Benchmark::run(const int &argc, const char *const *argv) {
  Benchmark::Result validation_result = validate_cla(argc, argv);
  if (!validation_result.ok) {
    std::cerr << validation_result.message << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<std::pair<int, int>> route_requests;
  Benchmark::Result parsing_result = parse_bench_file(bench_dir + argv[2], route_requests);
  if (!parsing_result.ok) {
    std::cerr << parsing_result.message << std::endl;
    exit(EXIT_FAILURE);
  }

  bench(graph_dir + argv[1], route_requests);
}

Benchmark::Result Benchmark::validate_cla(const int &argc,
                                          const char *const *argv) {
  if (argc != 3) {
    std::stringstream message_stream;
    message_stream << "Usage: " << argv[0]
                   << "<graph_file_name> <benchmark_file_name>";
    return {false, message_stream.str()};
  }

  std::ifstream graph_file(graph_dir + argv[1]);
  if (!graph_file) {
    return {false, "Graph file was not found in the 'data/graphs/' directory."};
  }
  graph_file.close();

  std::ifstream bench_file(bench_dir + argv[2]);
  if (!bench_file) {
    return {false, "Bench file was not found in the 'data/benchs/' directory."};
  }
  bench_file.close();

  return {true, "Validation of command line arguments was successful."};
}

Benchmark::Result
Benchmark::parse_bench_file(const std::string &bench_path,
                            std::vector<std::pair<int, int>> &route_requests) {
  std::string line;
  int src_id;
  int trg_id;

  std::ifstream bench_file(bench_path);
  if (!bench_file.is_open()) {
    return {false, "Error opening the file: " + bench_path + "."};
  }

  while (std::getline(bench_file, line)) {
    std::istringstream line_stream(line);
    if (!(line_stream >> src_id >> trg_id)) {
      return {false, "Error reading the line: " + line + "."};
    }
    route_requests.push_back({src_id, trg_id});
  }

  bench_file.close();

  return {true, "Parsing the bench file was successful."};
}

void Benchmark::bench(const std::string &graph_path,
                      const std::vector<std::pair<int, int>> &route_requests) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::cout;
  using std::endl;

  cout << "Starting graph parsing benchmark." << endl;
  cout << "----------------------------------------" << endl;

  std::vector<int> node_offsets;
  std::vector<Edge> edges;

  Graph_parser graph_parser;

  auto start_parser = Clock::now();
  graph_parser.parse(graph_path, node_offsets, edges);
  auto end_parser = Clock::now();
  auto duration_parser =
      std::chrono::duration_cast<Milliseconds>(end_parser - start_parser);

  cout << "Parsing of graph execution time: " << duration_parser.count()
       << "ms." << endl;
  cout << "----------------------------------------" << endl;
  cout << "Starting route request benchmark." << endl;
  cout << "----------------------------------------" << endl;

  long dijkstra_time = 0;
  std::vector<int> path_costs;
  Dijkstra dijsktra(node_offsets, edges);
  for (int i = 0; i < route_requests.size(); i++) {
    int src_id = route_requests[i].first;
    int trg_id = route_requests[i].second;

    auto start_dijsktra = Clock::now();
    DijkstraSingleResult dijkstra_result = dijsktra.src_to_trg(src_id, trg_id);
    auto end_dijkstra = Clock::now();
    auto duration_dijkstra =
        std::chrono::duration_cast<Milliseconds>(end_dijkstra - start_dijsktra);

    if (!dijkstra_result.success) {
      std::cerr << "Error while calculating best route using dijkstra." << endl;
      exit(EXIT_FAILURE);
    }
    cout << "\r" << i + 1 << "%" << std::flush;

    dijkstra_time += duration_dijkstra.count();
    path_costs.push_back(*dijkstra_result.cost);
  }
  cout << endl;

  cout << "Dijkstra average execution time: "
       << dijkstra_time / route_requests.size() << "ms." << endl;
  cout << "----------------------------------------" << endl;

  std::ofstream route_costs_file(std::string(BENCH_DIR) + "route_costs.txt");
  if (!route_costs_file) {
    std::cerr << "Error opening file." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs) {
    route_costs_file << cost << endl;
  }

  route_costs_file.close();
}
