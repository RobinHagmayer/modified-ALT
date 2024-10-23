#pragma once

#include <cstdint>
#include <vector>

namespace modified_alt {
namespace preprocessing {

using LandmarkDistanceVectors = std::vector<std::vector<uint32_t>>;

struct IndexCostPair {
  uint32_t index{};
  uint32_t cost{};
};

struct ModifiedAltData {
  std::vector<std::vector<uint32_t>> distances_between_landmarks{};
  // Stores for every node the nearest IndexCostPair from the landmark to each
  // node where the index is the nearest landmark index with the associated
  // cost.
  std::vector<IndexCostPair> from_landmark_to_nodes{};
  // Stores for every node the nearest IndexCostPair from each node to the
  // landmark where the index is the nearest landmark index with the associated
  // cost.
  std::vector<IndexCostPair> from_nodes_to_landmark{};
};

}  // namespace preprocessing
}  // namespace modified_alt
