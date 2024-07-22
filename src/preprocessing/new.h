#pragma once

#include "graph.h"

#include <cstddef>
#include <cstdint>
#include <vector>

struct New_lm_data {
  std::vector<uint32_t> landmark_ids;
  std::vector<std::pair<uint32_t, uint32_t>> closests_from_lm;
  std::vector<std::pair<uint32_t, uint32_t>> closests_to_lm;
  std::vector<std::vector<uint32_t>> dist_between_lms;

  New_lm_data() = default;

  New_lm_data(std::vector<uint32_t> landmark_ids,
              std::vector<std::pair<uint32_t, uint32_t>> closests_from_lm,
              std::vector<std::pair<uint32_t, uint32_t>> closests_to_lm,
              std::vector<std::vector<uint32_t>> dist_between_lms)
      : landmark_ids(landmark_ids), closests_from_lm(closests_from_lm),
        closests_to_lm(closests_to_lm), dist_between_lms(dist_between_lms) {}

  New_lm_data(New_lm_data &&other) noexcept
      : landmark_ids(std::move(other.landmark_ids)),
        closests_from_lm(std::move(other.closests_from_lm)),
        closests_to_lm(std::move(other.closests_to_lm)),
        dist_between_lms(std::move(other.dist_between_lms)) {}

  New_lm_data &operator=(New_lm_data &&other) noexcept {
    if (this != &other) {
      landmark_ids = std::move(other.landmark_ids);
      closests_from_lm = std::move(other.closests_from_lm);
      closests_to_lm = std::move(other.closests_to_lm);
      dist_between_lms = std::move(other.dist_between_lms);
    }
    return *this;
  }
};

class New {
public:
  static New_lm_data preprocess(size_t lm_count, const Graph &graph,
                                const Graph &graph_reverse);

  static void serialize(const std::string &file_path,
                        const New_lm_data &lm_data);

  static New_lm_data deserialize(const std::string &file_path);
};
