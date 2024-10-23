#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "preprocessing/data_types.h"

namespace modified_alt {
namespace algorithms {

class ModifiedAlt {
 public:
  ModifiedAlt(const graph::Graph graph,
              const preprocessing::ModifiedAltData modified_alt_data);

  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target) const;

  uint32_t ComputeSPSP(const uint32_t source, const uint32_t target,
                       uint32_t &visited_nodes) const;

 private:
  inline uint32_t h(const uint32_t node, const uint32_t target) const;

  const std::vector<uint32_t> offset_array_{};
  const std::vector<graph::Edge> edge_array_{};
  std::vector<std::vector<uint32_t>> distances_between_landmarks_{};
  std::vector<preprocessing::IndexCostPair> from_landmark_to_nodes_{};
  std::vector<preprocessing::IndexCostPair> from_nodes_to_landmark_{};
};

}  // namespace algorithms
}  // namespace modified_alt
