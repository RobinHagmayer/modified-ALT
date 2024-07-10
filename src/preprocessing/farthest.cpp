#include "farthest.h"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>

void Farthest::preprocess(
    const int &number_of_landmarks, Dijkstra &dijkstra_forward,
    Dijkstra &dijkstra_reverse,
    std::vector<std::vector<int>> &landmark_distances_forward,
    std::vector<std::vector<int>> &landmark_distances_reverse) {
  std::cout << "Starting preprocessing..." << std::endl;
  int number_of_nodes = dijkstra_reverse.node_offsets_.size() - 1;
  std::vector<int> landmarks;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  // Choose random node and calculate farthest node from it
  int random_node = distribution(rng);

  std::vector<int> random_node_distances(number_of_nodes, INT_MAX);
  dijkstra_forward.src_to_all(random_node, random_node_distances);

  std::vector<bool> reachable =
      check_reachability(dijkstra_forward, dijkstra_reverse, distribution(rng));

  int first_landmark = find_farthest_node(random_node_distances);
  if (!reachable[first_landmark]) {
    std::cout << "Nicht erreichbar" << std::endl;
  }
  std::cout << "First landmark: " << first_landmark << "\n";

  std::vector<int> to_first_landmark_distances(number_of_nodes, INT_MAX);
  dijkstra_reverse.src_to_all(first_landmark, to_first_landmark_distances);

  // Copy the first distances array
  std::vector<int> min_distances = to_first_landmark_distances;

  // Find next landmark
  int current_landmark = find_farthest_node(to_first_landmark_distances);
  std::cout << "Current landmark: " << current_landmark << "\n";
  if (!reachable[current_landmark]) {
    std::cout << "Nicht erreichbar" << std::endl;
  }

  landmark_distances_reverse.push_back(std::move(to_first_landmark_distances));

  // Calculate other landmarks based on the first landmark
  while (landmark_distances_reverse.size() < number_of_landmarks) {
    // Calculate distances from all nodes to a landmark
    std::vector<int> to_current_landmark_distances(number_of_nodes, INT_MAX);
    dijkstra_reverse.src_to_all(current_landmark,
                                to_current_landmark_distances);

    // Update minimum distance array
    for (int i = 0; i < min_distances.size(); i++) {
      if (min_distances[i] > to_current_landmark_distances[i]) {
        min_distances[i] = to_current_landmark_distances[i];
      }
    }

    landmark_distances_reverse.push_back(std::move(to_current_landmark_distances));

    // Find next landmark
    current_landmark = find_farthest_node(min_distances);
    if (!reachable[current_landmark]) {
      std::cout << "Nicht erreichbar" << std::endl;
    }
    std::cout << "Current landmark: " << current_landmark << "\n";
  }

  std::cout << "Preprocessing finished!\n";
}

int Farthest::find_farthest_node(
    const std::vector<int> &random_node_distances) {
  int max = INT_MIN;
  int landmark;

  for (int i = 0; i < random_node_distances.size(); i++) {
    int num = random_node_distances[i];

    if (num != INT_MAX && num > max) {
      max = num;
      landmark = i;
    }
  }

  if (max == INT_MIN) {
    return -1;
  }

  return landmark;
}

std::vector<bool> Farthest::check_reachability(Dijkstra &dijkstra_forward,
                                               Dijkstra &dijkstra_reverse,
                                               int node_id) {
  int number_of_nodes = dijkstra_forward.node_offsets_.size() - 1;
  std::vector<bool> reachable(number_of_nodes, 1);

  std::vector<int> forward_distances(number_of_nodes, INT_MAX);
  std::vector<int> reverse_distances(number_of_nodes, INT_MAX);

  dijkstra_forward.src_to_all(node_id, forward_distances);
  dijkstra_reverse.src_to_all(node_id, reverse_distances);

  int count_unreachable = 0;
  for (int i = 0; i < forward_distances.size(); i++) {
    if (forward_distances[i] == INT_MAX && reverse_distances[i] == INT_MAX) {
      count_unreachable++;
      reachable[i] = 0;
      /*std::cout << "Source node: " << node_id << ", unreachable node: " << i
       * << "\n";*/
    }
  }

  double unreachable_ratio =
      (double)count_unreachable / (double)number_of_nodes;
  /*std::cout << unreachable_ratio << std::endl;*/

  if (unreachable_ratio > 0.02) {
    std::cerr << "Preproccessor choose bad random node. Try again please."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return reachable;
}

void Farthest::serialize(
    const std::string &file_path,
    const std::vector<std::vector<int>> &landmark_distances) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t map_size = landmark_distances.size();
  output_file_stream.write(reinterpret_cast<const char *>(&map_size),
                           sizeof(map_size));

  for (const auto &vec : landmark_distances) {
    size_t vector_size = vec.size();
    output_file_stream.write(reinterpret_cast<const char *>(&vector_size),
                             sizeof(vector_size));

    output_file_stream.write(reinterpret_cast<const char *>(vec.data()),
                             vector_size * sizeof(int));
  }

  output_file_stream.close();
}

void Farthest::deserialize(
    const std::string &file_path,
    std::vector<std::vector<int>> &landmark_distances) {
  std::ifstream input_file_stream(file_path, std::ios::binary);

  if (!input_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t map_size;
  input_file_stream.read(reinterpret_cast<char *>(&map_size), sizeof(map_size));

  for (size_t i = 0; i < map_size; i++) {
    size_t vector_size;

    input_file_stream.read(reinterpret_cast<char *>(&vector_size),
                           sizeof(vector_size));

    std::vector<int> distances(vector_size);
    input_file_stream.read(reinterpret_cast<char *>(distances.data()),
                           vector_size * sizeof(int));

    landmark_distances.push_back(std::move(distances));
  }

  input_file_stream.close();
}
