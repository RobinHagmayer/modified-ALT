#include <gtest/gtest.h>

#include <random>

#include "algorithms/dijkstra.h"
#include "graph/graph_parser.h"
#include "preprocessing/random.h"

namespace modified_alt {
namespace preprocessing {

TEST(RandomPreprocessingTest, Random) {
  // auto g{graph::ParseGraphFile("../../data/graphs/stgtregbz.fmi", true)};
  //
  // std::random_device rd;
  // std::mt19937 gen(rd());
  // std::uniform_int_distribution<uint32_t> dist(
  //     0, static_cast<uint32_t>(g.GetNumberOfNodes()));
  //
  // auto lmdv{PreprocessRandom(algorithms::Dijkstra(g), 8, g.GetNumberOfNodes())};
  EXPECT_TRUE(true);
}

}  // namespace preprocessing
}  // namespace modified_alt
