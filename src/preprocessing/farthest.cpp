#include "farthest.h"

#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <utility>

Farthest_lm_data Farthest::preprocess(size_t lm_count, const Graph &graph,
                                      const Graph &graph_reverse) {
  std::cout << "Starting preprocessing..." << std::endl;

  std::vector<size_t> lm_indexes;
  std::vector<std::vector<uint32_t>> from_lm_distances;
  std::vector<std::vector<uint32_t>> to_lm_distances;

  lm_indexes.reserve(lm_count);
  from_lm_distances.reserve(lm_count);
  to_lm_distances.reserve(lm_count);

  Dijkstra dijkstra(graph);
  Dijkstra dijkstra_reverse(graph_reverse);

  size_t number_of_nodes = dijkstra_reverse.node_offsets.size() - 1;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<uint32_t> distribution(0, number_of_nodes);

  // Get a highly connected graph
  std::vector<bool> reachable =
      check_reachability(dijkstra, dijkstra_reverse, distribution(rng));

  int random_node;
  do {
    random_node = distribution(rng);
  } while (!reachable[random_node]);
  /*std::cout << "Random node ID: " << random_node << "\n";*/

  std::vector<uint32_t> random_node_distances =
      dijkstra.src_to_all(random_node);

  auto opt_first_lm = find_farthest_node(random_node_distances, reachable);

  if (!opt_first_lm.has_value() || !reachable[*opt_first_lm]) {
    std::cout << "Nicht erreichbar" << std::endl;
    exit(EXIT_FAILURE);
  }

  /*std::cout << "First landmark: " << *opt_first_lm << "\n";*/
  lm_indexes.push_back(0);

  std::vector<uint32_t> from_first_lm_dist = dijkstra.src_to_all(*opt_first_lm);
  std::vector<uint32_t> to_first_lm_dist =
      dijkstra_reverse.src_to_all(*opt_first_lm);

  // Copy the first distances array
  std::vector<uint32_t> min_distances = to_first_lm_dist;

  from_lm_distances.push_back(std::move(from_first_lm_dist));
  to_lm_distances.push_back(std::move(to_first_lm_dist));

  // Find next landmark
  auto opt_curr_lm = find_farthest_node(min_distances, reachable);
  if (!opt_curr_lm.has_value() || !reachable[*opt_curr_lm]) {
    std::cout << "Nicht erreichbar" << std::endl;
    exit(EXIT_FAILURE);
  }

  /*std::cout << "Current landmark: " << *opt_curr_lm << "\n";*/
  size_t lm_counter = 1;

  // Calculate other landmarks based on the first landmark
  while (to_lm_distances.size() < lm_count) {
    // Calculate distances from all nodes to a landmark
    std::vector<uint32_t> from_curr_lm_dist = dijkstra.src_to_all(*opt_curr_lm);
    std::vector<uint32_t> to_curr_lm_dist =
        dijkstra_reverse.src_to_all(*opt_curr_lm);

    // Update minimum distance array
    for (size_t i = 0; i < min_distances.size(); ++i) {
      if (min_distances[i] > to_curr_lm_dist[i]) {
        min_distances[i] = to_curr_lm_dist[i];
      }
    }

    lm_indexes.push_back(lm_counter);
    from_lm_distances.push_back(std::move(from_curr_lm_dist));
    to_lm_distances.push_back(std::move(to_curr_lm_dist));

    lm_counter++;

    // Find next landmark
    opt_curr_lm = find_farthest_node(min_distances, reachable);
    if (!opt_curr_lm.has_value() || !reachable[*opt_curr_lm]) {
      std::cout << "Nicht erreichbar" << std::endl;
      exit(EXIT_FAILURE);
    }
    /*std::cout << "Current landmark: " << *opt_curr_lm << "\n";*/
  }

  std::cout << "Preprocessing for farthest finished!\n";

  Farthest_lm_data lm_data;

  lm_data.lm_indexes = std::move(lm_indexes);
  lm_data.from_lm_distances = std::move(from_lm_distances);
  lm_data.to_lm_distances = std::move(to_lm_distances);

  return lm_data;
}

std::optional<uint32_t>
Farthest::find_farthest_node(const std::vector<uint32_t> &random_node_distances,
                             const std::vector<bool> &reachable) {
  uint32_t max = 0;
  uint32_t landmark;

  for (size_t i = 0; i < random_node_distances.size(); ++i) {
    if (!reachable[i]) {
      continue;
    }

    uint32_t num = random_node_distances[i];

    if (num != UINT32_MAX && num > max) {
      max = num;
      landmark = i;
    }
  }

  if (max == 0) {
    return std::nullopt;
  }

  return landmark;
}

std::vector<bool> Farthest::check_reachability(Dijkstra &dijkstra_forward,
                                               Dijkstra &dijkstra_reverse,
                                               uint32_t node_id) {
  size_t number_of_nodes = dijkstra_forward.node_offsets.size() - 1;
  std::vector<bool> reachable(number_of_nodes, 1);

  std::vector<uint32_t> forward_distances =
      dijkstra_forward.src_to_all(node_id);
  std::vector<uint32_t> reverse_distances =
      dijkstra_reverse.src_to_all(node_id);

  uint32_t count_unreachable = 0;
  for (size_t i = 0; i < forward_distances.size(); ++i) {
    if (forward_distances[i] == UINT32_MAX ||
        reverse_distances[i] == UINT32_MAX) {
      count_unreachable++;
      reachable[i] = 0;
      /*std::cout << "Source node: " << node_id << ", unreachable node: " << i
       * << "\n";*/
    }
  }

  double unreachable_ratio =
      (double)count_unreachable / (double)number_of_nodes;
  /*std::cout << unreachable_ratio << std::endl;*/

  if (unreachable_ratio > 0.05) {
    std::cerr << "Preproccessor choose bad random node. Try again please."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return reachable;
}

void Farthest::serialize(
    const std::string &file_path,
    const std::vector<std::vector<uint32_t>> &landmark_distances_forward,
    const std::vector<std::vector<uint32_t>> &landmark_distances_reverse) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t ldf_size = landmark_distances_forward.size();
  output_file_stream.write(reinterpret_cast<const char *>(&ldf_size),
                           sizeof(ldf_size));

  for (const auto &vec : landmark_distances_forward) {
    size_t vector_size = vec.size();
    output_file_stream.write(reinterpret_cast<const char *>(&vector_size),
                             sizeof(vector_size));

    output_file_stream.write(reinterpret_cast<const char *>(vec.data()),
                             vector_size * sizeof(uint32_t));
  }

  size_t ldr_size = landmark_distances_reverse.size();
  output_file_stream.write(reinterpret_cast<const char *>(&ldr_size),
                           sizeof(ldr_size));

  for (const auto &vec : landmark_distances_reverse) {
    size_t vector_size = vec.size();
    output_file_stream.write(reinterpret_cast<const char *>(&vector_size),
                             sizeof(vector_size));

    output_file_stream.write(reinterpret_cast<const char *>(vec.data()),
                             vector_size * sizeof(uint32_t));
  }

  output_file_stream.close();
}

void Farthest::deserialize(
    const std::string &file_path,
    std::vector<std::vector<uint32_t>> &landmark_distances_forward,
    std::vector<std::vector<uint32_t>> &landmark_distances_reverse) {
  std::ifstream input_file_stream(file_path, std::ios::binary);

  if (!input_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t ldf_size;
  input_file_stream.read(reinterpret_cast<char *>(&ldf_size), sizeof(ldf_size));

  for (size_t i = 0; i < ldf_size; i++) {
    size_t vector_size;

    input_file_stream.read(reinterpret_cast<char *>(&vector_size),
                           sizeof(vector_size));

    std::vector<uint32_t> distances_forward(vector_size);
    input_file_stream.read(reinterpret_cast<char *>(distances_forward.data()),
                           vector_size * sizeof(int));

    landmark_distances_forward.push_back(std::move(distances_forward));
  }

  size_t ldr_size;
  input_file_stream.read(reinterpret_cast<char *>(&ldr_size), sizeof(ldr_size));

  for (size_t i = 0; i < ldr_size; i++) {
    size_t vector_size;
    input_file_stream.read(reinterpret_cast<char *>(&vector_size),
                           sizeof(vector_size));

    std::vector<uint32_t> distances_reverse(vector_size);
    input_file_stream.read(reinterpret_cast<char *>(distances_reverse.data()),
                           vector_size * sizeof(int));

    landmark_distances_reverse.push_back(std::move(distances_reverse));
  }

  input_file_stream.close();
}
