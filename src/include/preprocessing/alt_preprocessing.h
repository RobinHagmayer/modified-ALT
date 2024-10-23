#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "algorithms/dijkstra.h"

namespace modified_alt {
namespace preprocessing {

using LandmarkDistanceVectors = std::vector<std::vector<uint32_t>>;

LandmarkDistanceVectors PreprocessRandom(
    const algorithms::Dijkstra &dijkstra_reverse, const size_t landmark_count,
    const size_t node_count);

}  // namespace preprocessing
}  // namespace modified_alt
