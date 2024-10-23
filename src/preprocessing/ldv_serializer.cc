#include "preprocessing/ldv_serializer.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

void SerializeLandmarkDistanceVectors(
    const std::string &path,
    const LandmarkDistanceVectors &landmark_distance_vectors) {
  std::ofstream out_file(path, std::ios::binary);

  if (!out_file) {
    std::cerr << "Error opening the preprocessing file: " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  const size_t landmark_distance_vectors_size{landmark_distance_vectors.size()};
  out_file.write(
      reinterpret_cast<const char *>(&landmark_distance_vectors_size),
      sizeof(landmark_distance_vectors_size));

  for (const auto &distance_vector : landmark_distance_vectors) {
    const size_t distance_vector_size{distance_vector.size()};

    out_file.write(reinterpret_cast<const char *>(&distance_vector_size),
                   sizeof(distance_vector_size));

    out_file.write(
        reinterpret_cast<const char *>(distance_vector.data()),
        static_cast<uint32_t>(distance_vector_size) * sizeof(uint32_t));
  }

  out_file.close();
}

LandmarkDistanceVectors DeserializeLandmarkDistanceVectors(
    const std::string &path) {
  LandmarkDistanceVectors landmark_distance_vectors{};
  std::ifstream in_file(path, std::ios::binary);

  if (!in_file) {
    std::cerr << "Error opening the preprocessing file: " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t landmark_distance_vectors_size{};
  in_file.read(reinterpret_cast<char *>(&landmark_distance_vectors_size),
               sizeof(landmark_distance_vectors_size));
  landmark_distance_vectors.reserve(landmark_distance_vectors_size);

  for (size_t i{0}; i < landmark_distance_vectors_size; ++i) {
    size_t distance_vector_size{};

    in_file.read(reinterpret_cast<char *>(&distance_vector_size),
                 sizeof(distance_vector_size));
    std::vector<uint32_t> distance_vector(distance_vector_size);

    in_file.read(
        reinterpret_cast<char *>(distance_vector.data()),
        static_cast<uint32_t>(distance_vector_size) * sizeof(uint32_t));

    landmark_distance_vectors.emplace_back(distance_vector);
  }

  in_file.close();

  return landmark_distance_vectors;
}

void Deserialize(
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
                           static_cast<uint32_t>(vector_size) * sizeof(int));

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
                           static_cast<uint32_t>(vector_size) * sizeof(int));

    landmark_distances_reverse.push_back(std::move(distances_reverse));
  }

  input_file_stream.close();
}

}  // namespace preprocessing
}  // namespace modified_alt
