#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "graph/graph.h"
#include "graph/graph_parser.h"

namespace modified_alt {
namespace graph {

class GraphTest : public testing::Test {
 protected:
  Graph toy_graph{ParseGraphFile("../../tests/data/toy.fmi")};
  Graph toy_graph_reverse{ParseGraphFile("../../tests/data/toy.fmi", true)};
  Graph example_graph{ParseGraphFile("../../data/graphs/example.fmi")};
};

TEST_F(GraphTest, GraphParser) {
  std::vector<uint32_t> correct_offset_array{0, 3, 3, 6, 7, 9};

  EXPECT_EQ(toy_graph.IsReverse(), false);
  EXPECT_EQ(correct_offset_array, toy_graph.GetOffsetArray());
}

TEST_F(GraphTest, GraphParserReverse) {
  std::vector<uint32_t> correct_offset_array{0, 1, 4, 6, 7, 9};
  std::vector<Edge> correct_edge_array{{0, 2, 6}, {1, 0, 9}, {1, 2, 5},
                                       {1, 4, 2}, {2, 0, 8}, {2, 3, 3},
                                       {3, 4, 1}, {4, 0, 7}, {4, 2, 4}};

  EXPECT_EQ(toy_graph_reverse.IsReverse(), true);
  EXPECT_EQ(correct_offset_array, toy_graph_reverse.GetOffsetArray());
  EXPECT_EQ(correct_edge_array, toy_graph_reverse.GetEdgeArray());
}

// TEST_F(GraphTest, GraphParser2) {
//   std::vector<uint32_t> correct_off_arr{0, 3, 3, 7, 9, 9, 9};
//   std::vector<Edge> correct_edge_arr{{0, 1, 9}, {0, 2, 4}, {0, 3, 5}, {2, 1,
//   2},
//                                      {2, 3, 1}, {2, 4, 7}, {2, 6, 8}, {3, 4,
//                                      1}, {3, 5, 3}, {6, 2, 4}};
//
//   EXPECT_EQ(correct_off_arr, example_graph.GetOffsetArray());
//   EXPECT_EQ(correct_edge_arr, example_graph.GetEdgeArray());
// }

}  // namespace graph
}  // namespace modified_alt
