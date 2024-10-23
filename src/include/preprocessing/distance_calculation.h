#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

LandmarkDistanceVectors CalculateLandmarkDistanceVectors(
    const graph::Graph &graph_reverse, const std::vector<uint32_t> &landmarks);

ModifiedAltData CalculateModifiedAltData(
    const graph::Graph &graph, const graph::Graph &graph_reverse,
    const std::vector<uint32_t> &landmarks);

}  // namespace preprocessing
}  // namespace modified_alt
