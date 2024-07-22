#pragma once

#include "graph.h"
#include "new.h"

#include <cstdint>
#include <vector>

class New_ALT {
public:
  /*const std::vector<uint32_t> &node_offsets;*/
  /*const std::vector<Edge> &edges;*/
  const Graph &graph;
  const std::vector<uint32_t> &landmark_ids;
  const std::vector<std::pair<uint32_t, uint32_t>> &closests_from_lm;
  const std::vector<std::pair<uint32_t, uint32_t>> &closests_to_lm;
  const std::vector<std::vector<uint32_t>> &dist_between_lms;

  /*New_ALT(const Graph &graph, const New_lm_data &lm_data)*/
  /*    : node_offsets(graph.node_offsets), edges(graph.edges),*/
  /*      landmark_ids(lm_data.landmark_ids),*/
  /*      closests_from_lm(lm_data.closests_from_lm),*/
  /*      closests_to_lm(lm_data.closests_to_lm),*/
  /*      dist_between_lms(lm_data.dist_between_lms){};*/

  New_ALT(const Graph &graph, const New_lm_data &lm_data)
      : graph(graph),
        landmark_ids(lm_data.landmark_ids),
        closests_from_lm(lm_data.closests_from_lm),
        closests_to_lm(lm_data.closests_to_lm),
        dist_between_lms(lm_data.dist_between_lms){};
  uint32_t src_to_trg(uint32_t src_id, uint32_t trg_id,
                      uint32_t &nodes_checked);

  uint32_t h(uint32_t node_id, uint32_t trg_id);
};
