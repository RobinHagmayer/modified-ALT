#include "graph/graph_partitioner.h"

#include <cstdint>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace graph {

std::vector<Cell> PartitionGraphInCells(const Graph &graph,
                                        const uint32_t cells_per_axis) {
  std::vector<graph::Vertex> vertices{graph.GetVertices()};
  Bounds bounds{graph.GetBounds()};

  // std::cout << "Max lat: " << bounds.max_lat << '\n';
  // std::cout << "Min lat: " << bounds.min_lat << '\n';
  // std::cout << "Max lon: " << bounds.max_lon << '\n';
  // std::cout << "Min lon: " << bounds.min_lon << '\n';

  double lat_range{bounds.max_lat - bounds.min_lat};
  double lon_range{bounds.max_lon - bounds.min_lon};

  // std::cout << "Lat range: " << lat_range << '\n';
  // std::cout << "Lon range: " << lon_range << '\n';

  double cell_lat_size{lat_range / cells_per_axis};
  double cell_lon_size{lon_range / cells_per_axis};

  // std::cout << "Cell lat size: " << cell_lat_size << '\n';
  // std::cout << "Cell lon size: " << cell_lon_size << '\n';

  std::vector<Cell> cells(cells_per_axis * cells_per_axis);
  for (uint32_t i{0}; i < cells.size(); ++i) {
    cells.at(i).index = i;
  }

  for (const Vertex &v : vertices) {
    uint32_t lat_index{
        static_cast<uint32_t>((v.lat - bounds.min_lat) / cell_lat_size)};
    uint32_t lon_index{
        static_cast<uint32_t>((v.lon - bounds.min_lon) / cell_lon_size)};

    if (lat_index == cells_per_axis) {
      --lat_index;
    }
    if (lon_index == cells_per_axis) {
      --lon_index;
    }

    uint32_t cell_index{lat_index * cells_per_axis + lon_index};

    if (cell_index <= cells.size()) {
      cells.at(cell_index).vertices.push_back(v);
    } else {
      // std::cout << v.lat << '\n';
      // std::cout << "Lat index: " << lat_index << '\n';
      // std::cout << "Lon index: " << lon_index << '\n';
      // std::cout << "Cell index: " << cell_index << '\n';
      std::cerr << "Cell index: " << cell_index
                << " exceeds size of cells vector: " << cells.size()
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return cells;
}

}  // namespace graph
}  // namespace modified_alt
