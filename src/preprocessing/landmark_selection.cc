#include "preprocessing/landmark_selection.h"

#include <cstddef>
#include <cstdint>
#include <random>
#include <unordered_set>
#include <vector>

#include "graph/graph.h"
#include "graph/graph_partitioner.h"
#include "preprocessing/utils.h"

namespace modified_alt {
namespace preprocessing {

std::vector<uint32_t> RandomLandmarkSelection(const graph::Graph &graph,
                                              const size_t landmark_count) {
  const size_t node_count{graph.GetNumberOfNodes()};
  std::vector<uint32_t> landmarks{};
  landmarks.reserve(node_count);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(
      0, static_cast<uint32_t>(node_count));

  std::vector<uint32_t> highly_reachable_distance_vector{
      preprocessing::CalculateHighlyReachableDistanceVector(graph, 98.0)};

  size_t remaining_landmarks{landmark_count};
  while (remaining_landmarks > 0) {
    const uint32_t random_node{dist(gen)};

    if (highly_reachable_distance_vector.at(random_node) == UINT32_MAX) {
      continue;
    }

    landmarks.emplace_back(random_node);

    --remaining_landmarks;
  }

  return landmarks;
}

std::vector<uint32_t> PartitionCornersLandmarkSelection(
    const std::vector<graph::Cell> &cells) {
  std::unordered_set<uint32_t> landmarks{};

  for (const graph::Cell &cell : cells) {
    // TODO: maybe adjust this because almost empty cells have probably all
    // vertices in one place.
    if (cell.vertices.size() <= 4) {
      continue;
    }

    double max_lat{-90.0}, min_lat{90.0}, max_lon{-180.0}, min_lon{180.0};
    uint32_t vertex_id_max_lat{}, vertex_id_min_lat{}, vertex_id_max_lon{},
        vertex_id_min_lon{};

    double second_max_lat{-90.0}, second_min_lat{90.0}, second_max_lon{-180.0},
        second_min_lon{180.0};
    uint32_t vertex_id_second_max_lat{}, vertex_id_second_min_lat{},
        vertex_id_second_max_lon{}, vertex_id_second_min_lon{};

    for (const graph::Vertex &vertex : cell.vertices) {
      if (vertex.lat > max_lat) {
        second_max_lat = max_lat;
        vertex_id_second_max_lat = vertex_id_max_lat;
        max_lat = vertex.lat;
        vertex_id_max_lat = vertex.id;
      } else if (vertex.lat > second_max_lat) {
        second_max_lat = vertex.lat;
        vertex_id_second_max_lat = vertex.id;
      }

      if (vertex.lat < min_lat) {
        second_min_lat = min_lat;
        vertex_id_second_min_lat = vertex_id_min_lat;
        min_lat = vertex.lat;
        vertex_id_min_lat = vertex.id;
      } else if (vertex.lat > second_min_lat) {
        second_min_lat = vertex.lat;
        vertex_id_second_min_lat = vertex.id;
      }

      if (vertex.lon > max_lon) {
        second_max_lon = max_lon;
        vertex_id_second_max_lon = vertex_id_max_lon;
        max_lon = vertex.lon;
        vertex_id_max_lon = vertex.id;
      } else if (vertex.lon > second_max_lon) {
        second_max_lon = vertex.lon;
        vertex_id_second_max_lon = vertex.id;
      }

      if (vertex.lon < min_lon) {
        second_min_lon = min_lon;
        vertex_id_second_min_lon = vertex_id_min_lon;
        min_lon = vertex.lon;
        vertex_id_min_lon = vertex.id;
      } else if (vertex.lon < second_min_lon) {
        second_min_lon = vertex.lon;
        vertex_id_second_min_lon = vertex.id;
      }
    }

    if (landmarks.count(vertex_id_max_lat)) {
      vertex_id_max_lat = vertex_id_second_max_lat;
    }
    landmarks.emplace(vertex_id_max_lat);

    if (landmarks.count(vertex_id_min_lat)) {
      vertex_id_min_lat = vertex_id_second_min_lat;
    }
    landmarks.emplace(vertex_id_min_lat);

    if (landmarks.count(vertex_id_max_lon)) {
      vertex_id_max_lon = vertex_id_second_max_lon;
    }
    landmarks.emplace(vertex_id_max_lon);

    if (landmarks.count(vertex_id_min_lon)) {
      vertex_id_min_lon = vertex_id_second_min_lon;
    }
    landmarks.emplace(vertex_id_min_lon);
  }

  return std::vector<uint32_t>(landmarks.begin(), landmarks.end());
}

}  // namespace preprocessing
}  // namespace modified_alt
