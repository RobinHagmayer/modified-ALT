#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace benchmark {

struct Query {
  uint32_t from{};
  uint32_t to{};
};

void GenerateBenchFiles(const std::string &selected_graph,
                        const graph::Graph &graph, const size_t query_count);

std::vector<Query> CreateRandomQueries(const graph::Graph &graph,
                                       const std::string &selected_graph,
                                       const size_t query_count);

void ComputeSolutions(const graph::Graph &graph,
                      const std::string &selected_graph,
                      const std::vector<Query> &queries);

std::vector<Query> ParseRouteRequestFile(const std::string &path);

void WriteCosts(const std::string &path, const std::vector<uint32_t> &costs);

}  // namespace benchmark
}  // namespace modified_alt
