#include "benchmark_file_generator.h"

#include <cstdlib>
#include <fstream>
#include <random>
#include <utility>

void Benchmark_file_generator::create_files(int number_of_requests,
                                            const std::string &graph) {
  std::cout << "Starting generating random benchmark files" << "\n";
  int number_of_nodes = dijkstra.node_offsets_.size();

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  std::vector<std::pair<int, int>> route_request(number_of_requests, {0, 0});
  std::vector<int> solutions(number_of_requests, 0);

  for (int i = 0; i < number_of_requests; i++) {
    int source = distribution(rng);
    int target = distribution(rng);
    route_request[i] = {source, target};

    int nodes_checked = 0;
    int solution = dijkstra.src_to_trg(source, target, nodes_checked);
    solutions[i] = solution;
  }

  // Write .que file
  std::ofstream request_file(std::string(BENCH_DIR) + "random_" + graph +
                             ".que");

  if (!request_file) {
    std::cerr << "Error writing benchmark '.que' file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto pair : route_request) {
    request_file << pair.first << " " << pair.second << "\n";
  }

  request_file.close();

  // Write .sol file
  std::ofstream solution_file(std::string(BENCH_DIR) + "random_" + graph +
                              ".sol");

  if (!solution_file) {
    std::cerr << "Error writing benchmark '.sol' file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto sol : solutions) {
    solution_file << sol << "\n";
  }

  solution_file.close();
  std::cout << "Finished generating random benchmark files" << "\n";
}
