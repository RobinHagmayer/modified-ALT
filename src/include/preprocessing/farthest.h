#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

LandmarkDistanceVectors SelectFarthestLandmarks(
    const graph::Graph &graph, const graph::Graph &graph_reverse,
    const size_t landmark_count);

uint32_t FindFarthestNode(
    const std::vector<uint32_t> &random_node_distances,
    const std::vector<uint32_t> &highly_reachable_distance_vector);

}  // namespace preprocessing
}  // namespace modified_alt
