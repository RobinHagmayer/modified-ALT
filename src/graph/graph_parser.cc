#include "graph/graph_parser.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "graph/graph.h"

namespace modified_alt {
namespace graph {

Graph ParseGraphFile(const std::string &path, const bool reverse) {
  std::string line{};

  std::ifstream graph_file(path);
  if (!graph_file) {
    std::cerr << "Error opening the graph file: " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  // Ignore comments and empty lines.
  while (getline(graph_file, line)) {
    if (!(line[0] == '#' || line.empty())) {
      break;
    }
  }

  size_t node_count{ReadCount(line, "Error reading the number of nodes.")};
  if (!getline(graph_file, line)) {
    std::cerr << "Error between reading the node and edge count." << std::endl;
    exit(EXIT_FAILURE);
  }
  size_t edge_count{ReadCount(line, "Error reading the number of edges.")};

  auto result{ReadVertices(graph_file, node_count)};
  std::vector<Edge> edge_array{ReadEdges(graph_file, edge_count, reverse)};

  // There should be no more lines at this point.
  if (getline(graph_file, line)) {
    std::cerr << "Error. There are still remaining lines in the file."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  graph_file.close();

  if (reverse) {
    std::sort(edge_array.begin(), edge_array.end(),
              [](const Edge &a, const Edge &b) {
                if (a.from == b.from) {
                  return a.to < b.to;
                }
                return a.from < b.from;
              });
  }

  return Graph(CreateOffsetArray(edge_array, node_count), edge_array,
               result.first, result.second, reverse);
}

inline size_t ReadCount(const std::string &line,
                        const std::string &error_message) {
  size_t count{};
  std::istringstream count_stream(line);
  if (!(count_stream >> count)) {
    std::cerr << error_message << std::endl;
    exit(EXIT_FAILURE);
  }

  return count;
}

inline std::pair<std::vector<Vertex>, Bounds> ReadVertices(
    std::ifstream &graph_file, const size_t node_count) {
  std::vector<Vertex> vertices(node_count);
  double max_lat{-90.0}, min_lat{90.0}, max_lon{-180.0}, min_lon{180.0};
  std::string line{};

  for (size_t i = 0; i < node_count; ++i) {
    if (!getline(graph_file, line)) {
      std::cerr << "Unexpected error at line: " << i + 1 << std::endl;
      std::cerr << "Line content: " << line << std::endl;
      exit(EXIT_FAILURE);
    }

    uint32_t id{};
    double lat{};
    double lon{};
    std::string temp2{};

    std::istringstream vertex_stream(line);
    if (!(vertex_stream >> id >> temp2 >> lat >> lon)) {
      std::cerr << "Error while parsing the edge line: " << line << std::endl;
      exit(EXIT_FAILURE);
    }

    if (lat > max_lat) {
      max_lat = lat;
    } else if (lat < min_lat) {
      min_lat = lat;
    }

    if (lon > max_lon) {
      max_lon = lon;
    } else if (lon < min_lon) {
      min_lon = lon;
    }

    vertices.at(i) = {id, lat, lon};
  }

  return {vertices, {max_lat, min_lat, max_lon, min_lon}};
}

inline std::vector<Edge> ReadEdges(std::ifstream &graph_file,
                                   const size_t edge_count,
                                   const bool reverse) {
  std::vector<Edge> edge_array{};
  edge_array.reserve(edge_count);
  std::string line{};

  for (size_t i = 0; i < edge_count; ++i) {
    if (!getline(graph_file, line)) {
      std::cerr << "Unexpected error at line: " << i + 1 << std::endl;
      std::cerr << "Line content: " << line << std::endl;
      exit(EXIT_FAILURE);
    }

    edge_array.emplace_back(ParseEdge(line, reverse));
  }

  return edge_array;
}

inline Edge ParseEdge(const std::string &line, const bool reverse) {
  uint32_t from{};
  uint32_t to{};
  uint32_t weight{};

  std::istringstream line_stream(line);
  if (!(line_stream >> from >> to >> weight)) {
    std::cerr << "Error while parsing the edge line: " << line << std::endl;
    exit(EXIT_FAILURE);
  }

  if (reverse) {
    return {to, from, weight};
  }

  return {from, to, weight};
}

inline std::vector<uint32_t> CreateOffsetArray(
    const std::vector<Edge> &edge_array, const size_t node_count) {
  // Count the number of edges for each node
  std::vector<uint32_t> offset_array(node_count + 1);

  for (const Edge &e : edge_array) {
    offset_array.at(e.from + 1) += 1;
  }

  // Cumulate the offsets
  for (size_t i = 1; i < offset_array.size(); ++i) {
    offset_array.at(i) += offset_array.at(i - 1);
  }

  return offset_array;
}

std::string ConvertToGeoJSON(const std::vector<Vertex> &vertices,
                             const std::vector<uint32_t> &selected_ids) {
  std::ostringstream geojson;
  geojson << std::fixed << std::setprecision(6);

  geojson << "{ \"type\": \"MultiPoint\", \"coordinates\": [";

  bool first = true;

  for (const uint32_t id : selected_ids) {
    for (const auto &vertex : vertices) {
      if (vertex.id == id) {
        if (!first) {
          geojson << ", ";
        }
        geojson << "[" << vertex.lon << ", " << vertex.lat << "]";
        first = false;
        break;
      }
    }
  }

  geojson << "] }";

  return geojson.str();
}
}  // namespace graph
}  // namespace modified_alt
