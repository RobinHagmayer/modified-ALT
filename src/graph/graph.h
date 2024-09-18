#pragma once

#include <ostream>
#include <vector>

/**
 * @brief Represents an edge of a weighted graph.
 *
 * The Edge struct contains the source id, the target id and the weight of an
 * edge.
 * (uint32_t src_id, uint32_t trg_id, uint32_t weight)
 * It also overloads the "<<" operator for a better output to stdout.
 */
struct Edge {
  uint32_t src_id;
  uint32_t trg_id;
  uint32_t weight;

  friend std::ostream &operator<<(std::ostream &os, const Edge &edge);
};

/**
 * @brief Represents a directed and weighted graph.
 *
 * The Graph struct contains a sorted vector of edges and an offset vector
 * which stores the first edge index for all vertices.
 * (std::vector<uint32_t> node_offsets, std::vector<Edge> edges)
 */
struct Graph {
  std::vector<uint32_t> node_offsets;
  std::vector<Edge> edges;
};
