#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "graph/graph_partitioner.h"

namespace modified_alt {
namespace preprocessing {

std::vector<uint32_t> RandomLandmarkSelection(const graph::Graph &graph,
                                              const size_t landmark_count);

std::vector<uint32_t> PartitionCornersLandmarkSelection(
    const std::vector<graph::Cell> &cells);

}  // namespace preprocessing
}  // namespace modified_alt
