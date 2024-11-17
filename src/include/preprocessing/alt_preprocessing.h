#pragma once

#include <cstddef>

#include "algorithms/dijkstra.h"
#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

LandmarkDistanceVectors PreprocessRandom(
    const algorithms::Dijkstra &dijkstra_reverse, const size_t landmark_count,
    const size_t node_count);

}  // namespace preprocessing
}  // namespace modified_alt
