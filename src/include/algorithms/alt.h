#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "preprocessing/alt_preprocessing.h"

namespace modified_alt {
namespace algorithms {

struct PQElement {
  uint32_t priority;
  uint32_t node;
  uint32_t distance;
};

// Represents default ALT algorithm.
// Can compute the Single-Source Shortest Path (SSSP) problem and
// the Single-Pair Shortest Path (SPSP) problem.
class ALT {
 public:
  ALT(const graph::Graph graph,
      preprocessing::LandmarkDistanceVectors landmark_distance_vectors);

  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target,
                       const size_t used_landmarks) const;

  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target,
                       const size_t used_landmarks,
                       uint32_t &visited_nodes) const;

  void SortLandmarkDistanceVectors(const uint32_t from, const uint32_t to);

 private:
  inline uint32_t h(const uint32_t node, const uint32_t target,
                    const size_t used_landmarks) const;

  const std::vector<uint32_t> offset_array_{};
  const std::vector<graph::Edge> edge_array_{};
  preprocessing::LandmarkDistanceVectors landmark_distance_vectors_{};
};

}  // namespace algorithms
}  // namespace modified_alt
