#pragma once

#include "dijkstra.h"

class Benchmark_file_generator {
public:
  Dijkstra dijkstra;

  Benchmark_file_generator(Dijkstra &dijkstra) : dijkstra(dijkstra) {}

  void create_files(int number_of_requests, const std::string &graph);
};
