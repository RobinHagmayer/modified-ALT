#include "benchmark.h"
#include "Dijkstra.h"
#include "GraphParser.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << "<graph_file_name> <benchmark_file_name>" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string bench_file_path =
      std::string(RESOURCE_PATH) + "/benchs/" + argv[2];

  // Check if graph file exists
  std::ifstream bench_file(bench_file_path);
  if (!bench_file) {
    std::cerr << "Error: File " << bench_file_path << " not found."
              << std::endl;
    return EXIT_FAILURE;
  }
  bench_file.close();

  Benchmark benchmark;
  BenchmarkParserResult bench_parser_result = benchmark.parse(bench_file_path);

  if (!bench_parser_result.success) {
    std::cerr << *bench_parser_result.error_message << std::endl;
    return EXIT_FAILURE;
  }

  const std::string graph_file_path =
      std::string(RESOURCE_PATH) + "/" + argv[1];
  benchmark.run(graph_file_path, *bench_parser_result.route_requests);
}

BenchmarkParserResult Benchmark::parse(const std::string &file_path) {
  BenchmarkParserResult bench_parser_result = {false, std::nullopt,
                                               std::nullopt};
  std::string line;
  int src_id;
  int trg_id;
  std::vector<std::pair<int, int>> route_requests;

  std::ifstream bench_file(file_path);
  if (!bench_file.is_open()) {
    bench_parser_result.error_message =
        "Error opening the file: " + file_path + ".";
    return bench_parser_result;
  }

  while (std::getline(bench_file, line)) {
    std::istringstream line_stream(line);
    if (!(line_stream >> src_id >> trg_id)) {
      bench_parser_result.error_message =
          "Error reading the line: " + line + ".";
      return bench_parser_result;
    }
    route_requests.push_back({src_id, trg_id});
  }

  bench_file.close();

  bench_parser_result.success = true;
  bench_parser_result.route_requests = route_requests;
  return bench_parser_result;
}

void Benchmark::run(const std::string &file_path,
                    const std::vector<std::pair<int, int>> &route_requests) {
  using std::cerr;
  using std::cout;
  using std::endl;
  using std::vector;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  cout << "Starting graph parsing benchmark." << endl;
  cout << "----------------------------------------" << endl;

  std::vector<int> node_offsets;
  std::vector<Edge> edges;

  GraphParser graph_parser = GraphParser();

  auto start_parser = high_resolution_clock::now();
  GraphParserResult graph_parser_result =
      graph_parser.parse(file_path, node_offsets, edges);
  auto end_parser = high_resolution_clock::now();
  auto duration_parser = duration_cast<milliseconds>(end_parser - start_parser);

  if (!graph_parser_result.success) {
    cerr << "Error while parsing the graph file." << endl;
    if (graph_parser_result.error_message.has_value()) {
      cerr << "Error message: " << *graph_parser_result.error_message << endl;
    }
    exit(EXIT_FAILURE);
  }

  cout << "Parsing of graph execution time: " << duration_parser.count()
       << "ms." << endl;
  cout << "----------------------------------------" << endl;
  cout << "Starting route request benchmark." << endl;
  cout << "----------------------------------------" << endl;

  long dijkstra_time = 0;
  vector<int> path_costs;
  Dijkstra dijsktra(node_offsets, edges);
  for (int i = 0; i < route_requests.size(); i++) {
    int src_id = route_requests[i].first;
    int trg_id = route_requests[i].second;

    auto start_dijsktra = high_resolution_clock::now();
    DijkstraSingleResult dijkstra_result = dijsktra.src_to_trg(src_id, trg_id);
    auto end_dijkstra = high_resolution_clock::now();
    auto duration_dijkstra =
        duration_cast<milliseconds>(end_dijkstra - start_dijsktra);

    if (!dijkstra_result.success) {
      cerr << "Error while calculating best route using dijkstra." << endl;
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

  std::ofstream route_costs_file(std::string(RESOURCE_PATH) +
                                 "/benchs/route_costs.txt");
  if (!route_costs_file) {
    cerr << "Error opening file." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto cost : path_costs) {
    route_costs_file << cost << endl;
  }

  route_costs_file.close();
}
