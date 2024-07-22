#pragma once

#include "graph.h"
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

struct Random_lm_data {
  std::vector<size_t> lm_indexes;
  std::vector<std::vector<uint32_t>> from_lm_distances;
  std::vector<std::vector<uint32_t>> to_lm_distances;

  Random_lm_data() = default;

  Random_lm_data(std::vector<size_t> lm_indexes,
                 std::vector<std::vector<uint32_t>> from_lm_distances,
                 std::vector<std::vector<uint32_t>> to_lm_distances)
      : lm_indexes(lm_indexes), from_lm_distances(from_lm_distances),
        to_lm_distances(to_lm_distances) {}

  Random_lm_data(Random_lm_data &&other) noexcept
      : lm_indexes(std::move(other.lm_indexes)),
        from_lm_distances(std::move(other.from_lm_distances)),
        to_lm_distances(std::move(other.to_lm_distances)) {}

  Random_lm_data &operator=(Random_lm_data &&other) noexcept {
    if (this != &other) {
      lm_indexes = std::move(other.lm_indexes);
      from_lm_distances = std::move(other.from_lm_distances);
      to_lm_distances = std::move(other.to_lm_distances);
    }
    return *this;
  }
};

class Random_landmarks {
public:
  static Random_lm_data preprocess(size_t lm_count, const Graph &graph,
                                   const Graph &graph_reverse);
};
