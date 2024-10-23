#include "preprocessing/modified-alt_preprocessing.h"

#include <omp.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "algorithms/dijkstra.h"
#include "graph/graph.h"

namespace modified_alt {
namespace preprocessing {

using algorithms::Dijkstra;
using graph::Graph;

ModifiedAltData PreprocessModifiedAlt(const Graph &graph,
                                      const Graph &graph_reverse,
                                      const std::vector<uint32_t> &landmarks) {
  const size_t landmark_count{landmarks.size()};
  const size_t node_count{graph.GetNumberOfNodes()};
  const Dijkstra dijkstra{graph};
  const Dijkstra dijkstra_reverse{graph_reverse};

  std::vector<std::vector<uint32_t>> distances_between_landmarks(
      landmark_count, std::vector<uint32_t>(landmark_count));
  std::vector<IndexCostPair> from_landmark_to_nodes(node_count,
                                                    {UINT32_MAX, UINT32_MAX});
  std::vector<IndexCostPair> from_nodes_to_landmark(node_count,
                                                    {UINT32_MAX, UINT32_MAX});

#pragma omp parallel for
  for (size_t i = 0; i < landmark_count; ++i) {
    uint32_t random_landmark_id{landmarks.at(i)};

    std::vector<uint32_t> distances_from_landmark_to_nodes{
        dijkstra.ComputeSSSP(random_landmark_id)};
    std::vector<uint32_t> distances_from_nodes_to_landmark{
        dijkstra_reverse.ComputeSSSP(random_landmark_id)};

    for (size_t j = 0; j < landmark_count; ++j) {
      distances_between_landmarks.at(i).at(j) =
          distances_from_landmark_to_nodes.at(landmarks.at(j));
    }

    for (size_t j = 0; j < node_count; ++j) {
      if (distances_from_landmark_to_nodes.at(j) <
          from_landmark_to_nodes.at(j).cost) {
        from_landmark_to_nodes.at(j) = {static_cast<uint32_t>(i),
                                        distances_from_landmark_to_nodes.at(j)};
      }

      if (distances_from_nodes_to_landmark.at(j) <
          from_nodes_to_landmark.at(j).cost) {
        from_nodes_to_landmark.at(j) = {static_cast<uint32_t>(i),
                                        distances_from_nodes_to_landmark.at(j)};
      }
    }
  }

  return {distances_between_landmarks, from_landmark_to_nodes,
          from_nodes_to_landmark};
}


}  // namespace preprocessing
}  // namespace modified_alt
