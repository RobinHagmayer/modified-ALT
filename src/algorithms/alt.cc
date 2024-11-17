#include "algorithms/alt.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace algorithms {

auto cmp = [](const PQElement &left, const PQElement &right) {
  return left.priority > right.priority;
};

using ALTPQ =
    std::priority_queue<PQElement, std::vector<PQElement>, decltype(cmp)>;

// Represents default ALT algorithm.
// Can compute the Single-Source Shortest Path (SSSP) problem and
// the Single-Pair Shortest Path (SPSP) problem.
ALT::ALT(const graph::Graph graph,
         preprocessing::LandmarkDistanceVectors landmark_distance_vectors)
    : offset_array_{graph.GetOffsetArray()},
      edge_array_{graph.GetEdgeArray()},
      landmark_distance_vectors_{landmark_distance_vectors} {}

uint32_t ALT::ComputeSPSP(const uint32_t source, const uint32_t target,
                          const size_t used_landmarks) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  ALTPQ pq(cmp);

  distances.at(source) = 0;
  pq.push({0, source, 0});

  while (!pq.empty()) {
    const uint32_t current_node{pq.top().node};
    const uint32_t current_distance{pq.top().distance};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    if (current_distance > distances.at(current_node)) {
      continue;
    }

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        // auto temp{h(edge_to_neighbor.to, target, 16)};
        // std::cout << temp << '\n';
        const uint32_t priority{new_distance +
                                h(edge_to_neighbor.to, target, used_landmarks)};
        pq.push(
            {priority, edge_to_neighbor.to, distances.at(edge_to_neighbor.to)});
      }
    }
  }

  return UINT32_MAX;
}

uint32_t ALT::ComputeSPSP(const uint32_t source, const uint32_t target,
                          const size_t used_landmarks,
                          uint32_t &visited_nodes) const {
  std::vector<uint32_t> distances(offset_array_.size() - 1, UINT32_MAX);
  ALTPQ pq(cmp);

  distances.at(source) = 0;
  pq.push({0, source, 0});

  while (!pq.empty()) {
    const uint32_t current_node{pq.top().node};
    const uint32_t current_distance{pq.top().distance};
    pq.pop();

    if (current_node == target) {
      return distances.at(target);
    }

    if (current_distance > distances.at(current_node)) {
      continue;
    }
    ++visited_nodes;

    const uint32_t first_edge_index{offset_array_.at(current_node)};
    const uint32_t next_node_edge_index{offset_array_.at(current_node + 1)};

    for (size_t i{first_edge_index}; i < next_node_edge_index; ++i) {
      const graph::Edge &edge_to_neighbor{edge_array_.at(i)};
      const uint32_t new_distance{distances.at(current_node) +
                                  edge_to_neighbor.weight};

      if (new_distance < distances.at(edge_to_neighbor.to)) {
        distances.at(edge_to_neighbor.to) = new_distance;
        // auto temp{h(edge_to_neighbor.to, target, 16)};
        // exit(0);
        // std::cout << temp << ' ' << std::flush;
        // std::this_thread::sleep_for(std::chrono::milliseconds(30));
        const uint32_t priority{new_distance +
                                h(edge_to_neighbor.to, target, used_landmarks)};
        pq.push(
            {priority, edge_to_neighbor.to, distances.at(edge_to_neighbor.to)});
      }
    }
  }

  return UINT32_MAX;
}

inline uint32_t ALT::h(const uint32_t node, const uint32_t target,
                       const size_t used_landmarks) const {
  // int64_t heuristic{0};
  uint32_t heuristic{0};

  for (size_t i{0}; i < used_landmarks; ++i) {
    const uint32_t node_to_landmark_distance{
        landmark_distance_vectors_.at(i).at(node)};
    const uint32_t target_to_landmark_distance{
        landmark_distance_vectors_.at(i).at(target)};

    if (node_to_landmark_distance == UINT32_MAX ||
        target_to_landmark_distance == UINT32_MAX) {
      continue;
    }

    // int64_t distance{static_cast<int64_t>(node_to_landmark_distance) -
    //                    static_cast<int64_t>(target_to_landmark_distance)};

    if (target_to_landmark_distance > node_to_landmark_distance) {
      continue;
    }

    const uint32_t distance{node_to_landmark_distance -
                            target_to_landmark_distance};

    if (distance > heuristic) {
      heuristic = distance;
    }
  }

  // return static_cast<uint32_t>(heuristic);
  return heuristic;
}

void ALT::SortLandmarkDistanceVectors(const uint32_t from, const uint32_t to) {
  // for (size_t j{0}; j < landmark_distance_vectors_.size(); ++j) {
  //   if (landmark_distance_vectors_.at(j).size() != 644199) {
  //     std::cout << from << to << '\n';
  //     std::cerr << "Error outside sort" << std::endl;
  //     exit(EXIT_FAILURE);
  //   }
  // }

  std::sort(landmark_distance_vectors_.begin(),
            landmark_distance_vectors_.end(),
            [from, to](const auto &a, const auto &b) {
              if (a.at(from) == UINT32_MAX || a.at(to) == UINT32_MAX) {
                return false;
              }

              if (b.at(from) == UINT32_MAX || b.at(to) == UINT32_MAX) {
                return true;
              }

              const int64_t diff_a{static_cast<int64_t>(a.at(from)) -
                                   static_cast<int64_t>(a.at(to))};
              const int64_t diff_b{static_cast<int64_t>(b.at(from)) -
                                   static_cast<int64_t>(b.at(to))};

              return diff_a > diff_b;
            });

  // std::sort(landmark_distance_vectors_.begin(),
  //           landmark_distance_vectors_.end(),
  //           // [](const auto &first, const auto &second) {
  //           [from, to](const auto &first, const auto &second) {
  //             assert(from < first.size());
  //             assert(to < first.size());
  //             assert(from < second.size());
  //             assert(to < second.size());
  //
  //             // std::cout << "first size: " << first.size() << " second
  //             size: "
  //             // << second.size() << '\n'; if (first.size() != 644199 ||
  //             // second.size() != 644199) {
  //             //   std::cerr << "Error inside sort" << std::endl;
  //             //   exit(EXIT_FAILURE);
  //             // }
  //             // return from < to;
  //             //
  //             long first_from{first.at(from)};
  //             long first_to{first.at(to)};
  //             long second_from{second.at(from)};
  //             long second_to{second.at(to)};
  //             // std::cout << "first from: " << first_from
  //             //           << ", first to: " << first_to
  //             //           << ", second from: " << second_from
  //             //           << ", second to: " << second_to << std::endl;
  //
  //             if (first_from == UINT32_MAX || first_to == UINT32_MAX) {
  //               return true;
  //             }
  //             if (second_from == UINT32_MAX || second_to == UINT32_MAX) {
  //               return false;
  //             }
  //
  //             long result_first{first_from - first_to};
  //             long result_second{second_from - second_to};
  //             // std::cout << "first: " << result_first
  //             //           << ", second: " << result_second << std::endl;
  //
  //             return result_first > result_second;
  //           });
}

}  // namespace algorithms
}  // namespace modified_alt
