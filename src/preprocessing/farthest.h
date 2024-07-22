#pragma once

#include "dijkstra.h"

#include <cstdint>
#include <optional>
#include <vector>

struct Farthest_lm_data {
  std::vector<size_t> lm_indexes;
  std::vector<std::vector<uint32_t>> from_lm_distances;
  std::vector<std::vector<uint32_t>> to_lm_distances;

  Farthest_lm_data() = default;

  Farthest_lm_data(std::vector<size_t> lm_indexes,
                   std::vector<std::vector<uint32_t>> from_lm_distances,
                   std::vector<std::vector<uint32_t>> to_lm_distances)
      : lm_indexes(lm_indexes), from_lm_distances(from_lm_distances),
        to_lm_distances(to_lm_distances) {}

  Farthest_lm_data(Farthest_lm_data &&other) noexcept
      : lm_indexes(std::move(other.lm_indexes)),
        from_lm_distances(std::move(other.from_lm_distances)),
        to_lm_distances(std::move(other.to_lm_distances)) {}

  Farthest_lm_data &operator=(Farthest_lm_data &&other) noexcept {
    if (this != &other) {
      lm_indexes = std::move(other.lm_indexes);
      from_lm_distances = std::move(other.from_lm_distances);
      to_lm_distances = std::move(other.to_lm_distances);
    }
    return *this;
  }
};

class Farthest {
public:
  static Farthest_lm_data preprocess(size_t lm_count, const Graph &graph,
                                     const Graph &graph_reverse);

  static std::optional<uint32_t>
  find_farthest_node(const std::vector<uint32_t> &random_node_distances,
                     const std::vector<bool> &reachable);

  static std::vector<bool> check_reachability(Dijkstra &dijkstra_forward,
                                              Dijkstra &dijkstra_reverse,
                                              uint32_t node_id);

  static void
  serialize(const std::string &file_path,
            const std::vector<std::vector<uint32_t>> &landmark_distances_forward,
            const std::vector<std::vector<uint32_t>> &landmark_distances_reverse);

  static void
  deserialize(const std::string &file_path,
              std::vector<std::vector<uint32_t>> &landmark_distances_forward,
              std::vector<std::vector<uint32_t>> &landmark_distances_reverse);
};
