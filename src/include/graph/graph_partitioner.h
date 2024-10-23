#pragma once

#include <cstdint>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace graph {

struct Cell {
  uint32_t index{};
  std::vector<graph::Vertex> vertices{};
};

std::vector<Cell> PartitionGraphInCells(const Graph &graph,
                                        const uint32_t cells_per_axis);

}  // namespace graph
}  // namespace modified_alt
