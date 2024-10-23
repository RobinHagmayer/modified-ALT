#include "graph/graph.h"

#include <cstdint>
#include <cstdlib>

namespace modified_alt {
namespace graph {

Graph::Graph(const std::vector<uint32_t> offset_array,
             const std::vector<Edge> edge_array,
             const std::vector<Vertex> vertices, const Bounds bounds,
             const bool reverse)
    : offset_array_{offset_array},
      edge_array_{edge_array},
      reverse_{reverse},
      vertices_{vertices},
      bounds_{bounds} {}

}  // namespace graph
}  // namespace modified_alt
