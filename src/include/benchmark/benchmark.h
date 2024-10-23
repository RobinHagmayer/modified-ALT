#pragma once

#include <cstdint>
#include <vector>

#include "benchmark/bench_utils.h"
#include "graph/graph.h"
#include "preprocessing/data_types.h"

namespace modified_alt {
namespace benchmark {

void run(int argc, char *argv[]);

void BenchDijkstra(const graph::Graph &graph,
                   const std::vector<Query> &route_requests);

void BenchAlt(const graph::Graph &graph,
              preprocessing::LandmarkDistanceVectors &landmark_distance_vectors,
              const std::vector<Query> &route_requests);

void BenchModifiedAlt(const graph::Graph &graph,
                      const preprocessing::ModifiedAltData &modified_alt_data,
                      const std::vector<Query> &route_requests);

void PrintBenchmarkHeader(const std::string &algorithm);

void PrintBenchmarkResults(const std::string &algorithm, const long time,
                           const uint32_t search_space,
                           const size_t query_count);

}  // namespace benchmark
}  // namespace modified_alt
