#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

namespace modified_alt {
namespace graph {

// Represents an edge of a directed and weighted graph.
// (int32_t from, int32_t to, int32_t weight)
struct Edge {
  uint32_t from{};
  uint32_t to{};
  uint32_t weight{};

  friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << "Edge from node: " << edge.from << " to node: " << edge.to
       << " with weight: " << edge.weight;
    return os;
  }

  bool operator==(const Edge& other) const {
    return (from == other.from) && (to == other.to) && (weight == other.weight);
  }
};

struct Vertex {
  uint32_t id{};
  double lat{};
  double lon{};
};

// Represents the geographical bounds of the graph.
struct Bounds {
  double max_lat{};
  double min_lat{};
  double max_lon{};
  double min_lon{};
};

// Represents a directed and weighted graph.
// The graph representation is a sorted list of edges (sorted by 'from') and
// an offset array which stores for each node the first index of the outgoing
// edges.
class Graph {
 public:
  Graph() = default;
  Graph(const std::vector<uint32_t>, const std::vector<Edge>,
        const std::vector<Vertex>, const Bounds, const bool);

  inline const std::vector<uint32_t> GetOffsetArray() const {
    return offset_array_;
  }

  inline size_t GetNumberOfNodes() const {
    if (offset_array_.size() == 0) {
      std::cerr << "Number of nodes cannot be negative." << std::endl;
      exit(EXIT_FAILURE);
    }
    return offset_array_.size() - 1;
  };

  inline const std::vector<Edge> GetEdgeArray() const { return edge_array_; }
  inline bool IsReverse() const { return reverse_; }
  inline const std::vector<Vertex> GetVertices() const { return vertices_; }
  inline const Bounds GetBounds() const { return bounds_; }

 private:
  const std::vector<uint32_t> offset_array_{};
  const std::vector<Edge> edge_array_{};
  const bool reverse_{};
  const std::vector<Vertex> vertices_{};
  const Bounds bounds_{};
};
}  // namespace graph
}  // namespace modified_alt
