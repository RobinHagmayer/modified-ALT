#pragma once

#include "dijkstra.h"

#include <cstdint>

class Benchmark_file_generator {
public:
  Dijkstra dijkstra;

  Benchmark_file_generator(Dijkstra &dijkstra) : dijkstra(dijkstra) {}

  void create_files(uint32_t number_of_requests, const std::string &graph);
};
