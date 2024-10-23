#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace preprocessing {

std::vector<uint32_t> CalculateHighlyReachableDistanceVector(
    const graph::Graph &graph_reverse, const double reachability_percentage);

}  // namespace preprocessing
}  // namespace modified_alt
