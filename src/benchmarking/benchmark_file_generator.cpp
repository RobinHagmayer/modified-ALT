#include "benchmark_file_generator.h"
#include "benchmark.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>

void Benchmark_file_generator::create_files(uint32_t number_of_requests,
                                            const std::string &graph_option) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, dijkstra.node_offsets.size() -
                                                      1);

  std::vector<std::pair<uint32_t, uint32_t>> route_request;
  route_request.reserve(number_of_requests);

  std::vector<uint32_t> solutions;
  solutions.reserve(number_of_requests);

  long total_time = 0;

  for (size_t i = 0; i < number_of_requests; ++i) {
    uint32_t source = distribution(rng);
    uint32_t target = distribution(rng);
    uint32_t nodes_checked = 0;

    route_request.push_back({source, target});
    auto start_dijsktra = Clock::now();
    auto cost = dijkstra.src_to_trg(source, target, nodes_checked);
    auto end_dijkstra = Clock::now();
    auto duration_dijkstra =
        std::chrono::duration_cast<Milliseconds>(end_dijkstra - start_dijsktra);
    total_time += duration_dijkstra.count();
    solutions.push_back(cost);

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / number_of_requests);
    std::cout << "\rProgress: " << progress << "%" << std::flush;
  }
  std::cout << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Dijkstra average execution time: "
            << total_time / number_of_requests << "ms." << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  // Write .que file
  std::ofstream request_file(BENCH_DIR + "random_" + graph_option + ".que");

  if (!request_file) {
    std::cerr << "Error writing benchmark '.que' file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto pair : route_request) {
    request_file << pair.first << " " << pair.second << "\n";
  }

  request_file.close();

  // Write .sol file
  std::ofstream solution_file(BENCH_DIR + "random_" + graph_option + ".sol");

  if (!solution_file) {
    std::cerr << "Error writing benchmark '.sol' file." << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto sol : solutions) {
    solution_file << sol << "\n";
  }

  solution_file.close();
}
