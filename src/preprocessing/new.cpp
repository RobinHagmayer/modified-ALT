#include "new.h"
#include "dijkstra.h"

#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <utility>
#include <vector>

New_lm_data New::preprocess(size_t lm_count, const Graph &graph,
                            const Graph &graph_reverse) {
  std::cout << "Starting preprocessing for new ALT" << std::endl;

  Dijkstra dijkstra(graph);
  // FEHLER
  Dijkstra dijkstra_reverse(graph_reverse);

  std::vector<uint32_t> random_lms;
  // Stores closest landmark idx from lm to all other nodes
  std::vector<std::pair<uint32_t, uint32_t>> closests_from_lm;
  // Stores closest landmark idx from nodes to lm
  std::vector<std::pair<uint32_t, uint32_t>> closests_to_lm;
  // Store distance between two lms, [i][j] means from i to j
  std::vector<std::vector<uint32_t>> dist_between_lms;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<uint32_t> distribution(
      0, graph.node_offsets.size() - 1);

  random_lms.reserve(lm_count);
  dist_between_lms.assign(lm_count, std::vector<uint32_t>(lm_count));
  closests_from_lm.resize(graph.node_offsets.size() - 1,
                          {UINT32_MAX, UINT32_MAX});
  closests_to_lm.resize(graph.node_offsets.size() - 1,
                        {UINT32_MAX, UINT32_MAX});

  for (size_t i = 0; i < lm_count; ++i) {
    uint32_t random_lm_id = distribution(rng);
    random_lms.push_back(random_lm_id);
  }

  #pragma omp parallel for
  for (size_t i = 0; i < lm_count; ++i) {
    uint32_t random_lm_id = random_lms[i];

    // From lm to all nodes
    auto dist_from = dijkstra.src_to_all(random_lm_id);
    // From all nodes to lm
    auto dist_to = dijkstra_reverse.src_to_all(random_lm_id);

    for (size_t j = 0; j < lm_count; ++j) {
      dist_between_lms[i][j] = dist_from[random_lms[j]];
    }

    for (size_t j = 0; j < dist_from.size(); ++j) {
      // data race
      if (dist_from[j] < closests_from_lm[j].second) {
        closests_from_lm[j] = {i, dist_from[j]};
      }

      // data race
      if (dist_to[j] < closests_to_lm[j].second) {
        closests_to_lm[j] = {i, dist_to[j]};
      }
    }
    /*uint32_t progress = static_cast<uint32_t>((i + 1) * 100 / lm_count);*/
    /*std::cout << "\rProgress: " << progress << "%" << std::flush;*/
  }

  std::cout << std::endl;
  std::cout << "Finished preprocessing for new ALT" << std::endl;

  New_lm_data lm_data;

  lm_data.landmark_ids = std::move(random_lms);
  lm_data.closests_from_lm = std::move(closests_from_lm);
  lm_data.closests_to_lm = std::move(closests_to_lm);
  lm_data.dist_between_lms = std::move(dist_between_lms);

  return lm_data;
}

void New::serialize(const std::string &file_path, const New_lm_data &lm_data) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  uint32_t size = lm_data.landmark_ids.size();
  output_file_stream.write(reinterpret_cast<const char *>(&size), sizeof(size));
  output_file_stream.write(
      reinterpret_cast<const char *>(lm_data.landmark_ids.data()),
      size * sizeof(uint32_t));

  uint32_t size1 = lm_data.closests_from_lm.size();
  output_file_stream.write(reinterpret_cast<const char *>(&size1),
                           sizeof(size1));
  for (const auto &p : lm_data.closests_from_lm) {
    output_file_stream.write(reinterpret_cast<const char *>(&p.first),
                             sizeof(p.first));
    output_file_stream.write(reinterpret_cast<const char *>(&p.second),
                             sizeof(p.second));
  }

  uint32_t size2 = lm_data.closests_to_lm.size();
  output_file_stream.write(reinterpret_cast<const char *>(&size2),
                           sizeof(size2));
  for (const auto &p : lm_data.closests_to_lm) {
    output_file_stream.write(reinterpret_cast<const char *>(&p.first),
                             sizeof(p.first));
    output_file_stream.write(reinterpret_cast<const char *>(&p.second),
                             sizeof(p.second));
  }

  uint32_t size3 = lm_data.dist_between_lms.size();
  output_file_stream.write(reinterpret_cast<const char *>(&size3),
                           sizeof(size3));
  for (const auto &innerVec : lm_data.dist_between_lms) {
    uint32_t innerSize = innerVec.size();
    output_file_stream.write(reinterpret_cast<const char *>(&innerSize),
                             sizeof(innerSize));
    output_file_stream.write(reinterpret_cast<const char *>(innerVec.data()),
                             innerSize * sizeof(uint32_t));
  }

  output_file_stream.close();
}

New_lm_data New::deserialize(const std::string &file_path) {
  std::vector<uint32_t> landmark_ids;
  std::vector<std::pair<uint32_t, uint32_t>> closests_from_lm;
  std::vector<std::pair<uint32_t, uint32_t>> closests_to_lm;
  std::vector<std::vector<uint32_t>> dist_between_lms;

  std::ifstream input_file_stream(file_path, std::ios::binary);

  if (!input_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  uint32_t size;
  input_file_stream.read(reinterpret_cast<char *>(&size), sizeof(size));
  landmark_ids.resize(size);
  input_file_stream.read(reinterpret_cast<char *>(landmark_ids.data()), size * sizeof(uint32_t));

  uint32_t size1;
  input_file_stream.read(reinterpret_cast<char *>(&size1), sizeof(size1));
  closests_from_lm.resize(size1);
  for (auto &p : closests_from_lm) {
    input_file_stream.read(reinterpret_cast<char *>(&p.first), sizeof(p.first));
    input_file_stream.read(reinterpret_cast<char *>(&p.second),
                           sizeof(p.second));
  }

  uint32_t size2;
  input_file_stream.read(reinterpret_cast<char *>(&size2), sizeof(size2));
  closests_to_lm.resize(size2);
  for (auto &p : closests_to_lm) {
    input_file_stream.read(reinterpret_cast<char *>(&p.first), sizeof(p.first));
    input_file_stream.read(reinterpret_cast<char *>(&p.second),
                           sizeof(p.second));
  }

  uint32_t size3;
  input_file_stream.read(reinterpret_cast<char *>(&size3), sizeof(size3));
  dist_between_lms.resize(size3);
  for (auto &innerVec : dist_between_lms) {
    uint32_t innerSize;
    input_file_stream.read(reinterpret_cast<char *>(&innerSize),
                           sizeof(innerSize));
    innerVec.resize(innerSize);
    input_file_stream.read(reinterpret_cast<char *>(innerVec.data()),
                           innerSize * sizeof(uint32_t));
  }

  input_file_stream.close();

  New_lm_data lm_data;

  lm_data.landmark_ids = std::move(landmark_ids);
  lm_data.closests_from_lm = std::move(closests_from_lm);
  lm_data.closests_to_lm = std::move(closests_to_lm);
  lm_data.dist_between_lms = std::move(dist_between_lms);

  return lm_data;
}
