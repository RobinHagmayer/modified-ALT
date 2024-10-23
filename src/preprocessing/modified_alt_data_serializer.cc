#include "preprocessing/modified_alt_data_serializer.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

void SerializeModifiedAltData(const std::string &path,
                              const ModifiedAltData &data) {
  std::ofstream out(path, std::ios::binary);

  if (!out) {
    std::cerr << "Error opening the file: " << path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t size1{data.distances_between_landmarks.size()};
  out.write(reinterpret_cast<const char *>(&size1), sizeof(size1));

  for (const auto &inner_vec : data.distances_between_landmarks) {
    size_t inner_size{inner_vec.size()};
    out.write(reinterpret_cast<const char *>(&inner_size), sizeof(inner_size));
    out.write(reinterpret_cast<const char *>(inner_vec.data()),
              static_cast<std::streamsize>(inner_size * sizeof(uint32_t)));
  }

  size_t size2{data.from_landmark_to_nodes.size()};
  out.write(reinterpret_cast<const char *>(&size2), sizeof(size2));
  out.write(reinterpret_cast<const char *>(data.from_landmark_to_nodes.data()),
            static_cast<std::streamsize>(size2 * sizeof(IndexCostPair)));

  size_t size3{data.from_nodes_to_landmark.size()};
  out.write(reinterpret_cast<const char *>(&size3), sizeof(size3));
  out.write(reinterpret_cast<const char *>(data.from_nodes_to_landmark.data()),
            static_cast<std::streamsize>(size3 * sizeof(IndexCostPair)));

  out.close();
}

ModifiedAltData DeserializeModifiedAltData(const std::string &path) {
  std::vector<std::vector<uint32_t>> distance_between_landmarks{};
  std::vector<IndexCostPair> from_landmark_to_nodes;
  std::vector<IndexCostPair> from_nodes_to_landmark;

  std::ifstream input(path, std::ios::binary);

  if (!input) {
    std::cerr << "Error opening the file: " << path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t size1{};
  input.read(reinterpret_cast<char *>(&size1), sizeof(size1));
  distance_between_landmarks.resize(size1);
  for (size_t i{0}; i < size1; ++i) {
    size_t inner_size{};
    input.read(reinterpret_cast<char *>(&inner_size), sizeof(inner_size));
    distance_between_landmarks.at(i).resize(inner_size);
    input.read(
        reinterpret_cast<char *>(distance_between_landmarks.at(i).data()),
        static_cast<std::streamsize>(inner_size * sizeof(uint32_t)));
  }

  size_t size2{};
  input.read(reinterpret_cast<char *>(&size2), sizeof(size2));
  from_landmark_to_nodes.resize(size2);
  input.read(reinterpret_cast<char *>(from_landmark_to_nodes.data()),
             static_cast<std::streamsize>(size2 * sizeof(IndexCostPair)));

  size_t size3{};
  input.read(reinterpret_cast<char *>(&size3), sizeof(size3));
  from_nodes_to_landmark.resize(size3);
  input.read(reinterpret_cast<char *>(from_nodes_to_landmark.data()),
             static_cast<std::streamsize>(size3 * sizeof(IndexCostPair)));

  input.close();

  return {distance_between_landmarks, from_landmark_to_nodes,
          from_nodes_to_landmark};
}

}  // namespace preprocessing
}  // namespace modified_alt
