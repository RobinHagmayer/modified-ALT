#pragma once

#include <string>

#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

void SerializeLandmarkDistanceVectors(
    const std::string &path,
    const LandmarkDistanceVectors &landmark_distance_vectors);

LandmarkDistanceVectors DeserializeLandmarkDistanceVectors(
    const std::string &path);

void Deserialize(
    const std::string &file_path,
    std::vector<std::vector<uint32_t>> &landmark_distances_forward,
    std::vector<std::vector<uint32_t>> &landmark_distances_reverse);

}  // namespace preprocessing
}  // namespace modified_alt
