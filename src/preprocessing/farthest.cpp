#include "farthest.h"

#include <algorithm>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <random>

void Farthest::preprocess(
    const int &number_of_landmarks, Dijkstra &dijkstra_forward,
    Dijkstra &dijkstra_reverse,
    std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
    std::unordered_map<int, std::vector<int>> &landmark_distances_reverse) {
  std::cout << "Starting preprocessing..." << std::endl;
  int number_of_nodes = dijkstra_reverse.node_offsets_.size() - 1;
  std::vector<int> landmarks;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  int random_node = distribution(rng);
  landmarks.push_back(random_node);

  std::vector<int> random_landmark_distances_reverse(number_of_nodes, INT_MAX);
  dijkstra_reverse.src_to_all(random_node, random_landmark_distances_reverse);
  landmark_distances_reverse[random_node] = random_landmark_distances_reverse;

  std::vector<int> random_landmark_distance_forward(number_of_nodes, INT_MAX);
  dijkstra_forward.src_to_all(random_node, random_landmark_distance_forward);

  while (landmarks.size() < number_of_landmarks) {
    int max_min_distance = -1;
    int next_landmark = -1;

    // Check all nodes to all landmarks
    for (int i = 0; i < number_of_nodes; i++) {
      int count = std::count(landmarks.begin(), landmarks.end(), i);
      // If current node is already a landmark, skip it
      if (count > 0) {
        continue;
      }

      int min_distance = INT_MAX;
      for (auto landmark : landmarks) {
        int distance = landmark_distances_reverse[landmark][i];

        if (distance == INT_MAX || distance == -1) {
          continue;
        }

        if (distance < min_distance) {
          min_distance = distance;
        }
      }

      if (min_distance < INT_MAX && min_distance > max_min_distance) {
        max_min_distance = min_distance;
        next_landmark = i;
      }
    }

    if (next_landmark > -1) {
      std::cout << "Landmark: " << next_landmark << "\n";
      landmarks.push_back(next_landmark);
      std::vector<int> distances_reverse(number_of_nodes, INT_MAX);
      dijkstra_reverse.src_to_all(next_landmark, distances_reverse);
      landmark_distances_reverse[next_landmark] = distances_reverse;

      std::vector<int> distances_forward(number_of_nodes, INT_MAX);
      dijkstra_forward.src_to_all(next_landmark, distances_forward);
      landmark_distances_forward[next_landmark] = distances_forward;
    }
  }

  std::cout << "Preprocessing finished!\n";
  std::cout << "----------------------------------------" << std::endl;
}

void Farthest::serialize(
    const std::string &file_path,
    const std::unordered_map<int, std::vector<int>> &landmark_distances) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t map_size = landmark_distances.size();
  output_file_stream.write(reinterpret_cast<const char *>(&map_size),
                           sizeof(map_size));

  for (const auto &pair : landmark_distances) {
    output_file_stream.write(reinterpret_cast<const char *>(&pair.first),
                             sizeof(pair.first));

    size_t vector_size = pair.second.size();
    output_file_stream.write(reinterpret_cast<const char *>(&vector_size),
                             sizeof(vector_size));

    output_file_stream.write(reinterpret_cast<const char *>(pair.second.data()),
                             vector_size * sizeof(int));
  }

  output_file_stream.close();
}

void Farthest::deserialize(
    const std::string &file_path,
    std::unordered_map<int, std::vector<int>> &landmark_distances) {
  std::ifstream input_file_stream(file_path, std::ios::binary);

  if (!input_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t map_size;
  input_file_stream.read(reinterpret_cast<char *>(&map_size), sizeof(map_size));

  for (size_t i = 0; i < map_size; i++) {
    int key;
    size_t vector_size;

    input_file_stream.read(reinterpret_cast<char *>(&key), sizeof(key));

    input_file_stream.read(reinterpret_cast<char *>(&vector_size),
                           sizeof(vector_size));

    std::vector<int> distances(vector_size);
    input_file_stream.read(reinterpret_cast<char *>(distances.data()),
                           vector_size * sizeof(int));

    landmark_distances[key] = distances;
  }

  input_file_stream.close();
}
