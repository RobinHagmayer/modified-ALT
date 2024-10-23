#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace algorithms {

// Represents Dijkstra's algorithm.
// Can compute the Single-Source Shortest Path (SSSP) problem and
// the Single-Pair Shortest Path (SPSP) problem.
class Dijkstra {
 public:
  Dijkstra(const graph::Graph graph);

  std::vector<uint32_t> ComputeSSSP(const uint32_t source) const;
  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target) const;

  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target,
                       uint32_t &visited_nodes) const;

 private:
  const std::vector<uint32_t> offset_array_{};
  const std::vector<graph::Edge> edge_array_{};
};

}  // namespace algorithms
}  // namespace modified_alt
