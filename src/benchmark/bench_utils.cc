#include "benchmark/bench_utils.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "algorithms/dijkstra.h"
#include "constants.h"
#include "graph/graph.h"

namespace modified_alt {
namespace benchmark {

using algorithms::Dijkstra;
using graph::Graph;

void GenerateBenchFiles(const std::string &selected_graph, const Graph &graph,
                        const size_t query_count) {
  using std::cout;

  cout << "Generating " << query_count << " random queries ...\n";
  std::vector<Query> queries{
      CreateRandomQueries(graph, selected_graph, query_count)};
  cout << "Finished generating " << query_count << " random queries!\n\n";

  cout << "Computing solutions ...\n";
  ComputeSolutions(graph, selected_graph, queries);
  cout << "Finished computing solutions!\n";
}

std::vector<Query> CreateRandomQueries(const Graph &graph,
                                       const std::string &selected_graph,
                                       const size_t query_count) {
  std::vector<Query> queries(query_count);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>(graph.GetNumberOfNodes()));

  std::ofstream query_file(BENCH_FILES_DIR + selected_graph + ".que");
  if (!query_file) {
    std::cerr << "Error opening the query file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (size_t i{0}; i < query_count; ++i) {
    uint32_t from{dist(gen)};
    uint32_t to{dist(gen)};
    queries.at(i) = {from, to};

    query_file << from << ' ' << to << '\n';
  }

  query_file.close();

  return queries;
}

void ComputeSolutions(const Graph &graph, const std::string &selected_graph,
                      const std::vector<Query> &queries) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::chrono::duration_cast;

  Dijkstra dijkstra(graph);

  std::ofstream solution_file(BENCH_FILES_DIR + selected_graph + ".sol");
  if (!solution_file) {
    std::cerr << "Error opening the solution file." << std::endl;
    exit(EXIT_FAILURE);
  }

  double i{1.0};
  long time{};
  for (const Query &q : queries) {
    auto start{Clock::now()};
    uint32_t path_cost{dijkstra.ComputeSPSP(q.from, q.to)};
    auto end{Clock::now()};
    time += duration_cast<Milliseconds>(end - start).count();

    solution_file << path_cost << '\n';

    uint32_t progress{static_cast<uint32_t>(
        (i / static_cast<double>(queries.size())) * 100.0)};
    std::cout << "\rProgress: " << progress << "%" << std::flush;
    ++i;
  }

  solution_file.close();
  std::cout << std::endl;
  std::cout << "Average computation time: "
            << static_cast<size_t>(time) / queries.size() << " ms.\n";
}

std::vector<Query> ParseRouteRequestFile(const std::string &path) {
  std::vector<Query> route_requests{};
  std::string line{};
  uint32_t from{};
  uint32_t to{};

  std::ifstream route_request_file(path);
  if (!route_request_file.is_open()) {
    std::cerr << "Error opening the file: " + path << std::endl;
    exit(EXIT_FAILURE);
  }

  while (std::getline(route_request_file, line)) {
    std::istringstream line_stream(line);

    if (!(line_stream >> from >> to)) {
      std::cerr << "Error reading the line: " + line + ".\n";
      exit(EXIT_FAILURE);
    }

    route_requests.push_back({from, to});
  }

  route_request_file.close();

  return route_requests;
}

void WriteCosts(const std::string &path, const std::vector<uint32_t> &costs) {
  std::ofstream cost_file(path);

  if (!cost_file) {
    std::cerr << "Error opening \"" << path << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (const uint32_t &cost : costs) {
    cost_file << cost << '\n';
  }

  cost_file.close();
}

}  // namespace benchmark
}  // namespace modified_alt
