#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace graph {

// Set reverse to true if you want to flip the edges.
// Usefull for making "from all nodes to a single node" shortest path queries.
Graph ParseGraphFile(const std::string &, const bool = false);

inline size_t ReadCount(const std::string &, const std::string &);
inline std::pair<std::vector<Vertex>, Bounds> ReadVertices(std::ifstream &,
                                                           const size_t);
inline std::vector<Edge> ReadEdges(std::ifstream &, const size_t, const bool);
inline Edge ParseEdge(const std::string &, const bool);
inline std::vector<uint32_t> CreateOffsetArray(const std::vector<Edge> &,
                                               const size_t);

std::string ConvertToGeoJSON(const std::vector<Vertex> &vertices,
                             const std::vector<uint32_t> &selected_ids);

}  // namespace graph
}  // namespace modified_alt
