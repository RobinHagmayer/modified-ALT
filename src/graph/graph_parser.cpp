#include "graph_parser.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

void Graph_parser::parse_edges(const std::string &line,
                               std::vector<Edge> &edges, bool reverse) {
  uint32_t src_id;
  uint32_t trg_id;
  uint32_t weight;
  Edge edge;

  std::istringstream line_stream(line);
  line_stream >> src_id >> trg_id >> weight;

  if (reverse) {
    edge = {trg_id, src_id, weight};
  } else {
    edge = {src_id, trg_id, weight};
  }

  edges.push_back(edge);
}

void Graph_parser::create_offset_array(const std::vector<Edge> &edges,
                                       std::vector<uint32_t> &node_offsets) {
  uint32_t curr_node = edges[0].src_id;
  node_offsets.push_back(curr_node);

  for (size_t i = 0; i < edges.size(); ++i) {
    uint32_t src_id = edges[i].src_id;
    uint32_t trg_id = edges[i].trg_id;

    if (curr_node + 1 == src_id) {
      node_offsets.push_back(i);
      curr_node++;
    } else if (curr_node + 1 < src_id) {
      uint32_t nodes_to_fill = src_id - curr_node;
      for (size_t j = 0; j < nodes_to_fill; ++j) {
        node_offsets.push_back(i);
      }
      curr_node = src_id;
    }
  }
}

Graph Graph_parser::parse(const std::string &file_path, bool reverse) {
  std::vector<uint32_t> node_offsets;
  std::vector<Edge> edges;

  std::string line;
  size_t node_count;
  size_t edge_count;

  std::ifstream graph_file(file_path);

  if (!graph_file.is_open()) {
    std::cerr << "Error opening the file: " << file_path << std::endl;
    exit(EXIT_FAILURE);
  }

  // Ignore comments and empty lines
  while (std::getline(graph_file, line)) {
    if (!(line[0] == '#' || line.empty())) {
      break;
    }
  }

  // Read number of nodes
  std::istringstream node_count_stream(line);
  if (!(node_count_stream >> node_count)) {
    std::cerr << "Error reading the number of nodes." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Move to next line
  if (!std::getline(graph_file, line)) {
    std::cerr
        << "Unexpected error between reading number of nodes and number of "
           "edges."
        << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read number of edges
  std::istringstream edge_count_stream(line);
  if (!(edge_count_stream >> edge_count)) {
    std::cerr << "Error reading the number of edges." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read all nodes
  for (size_t i = 0; i < node_count; ++i) {
    if (!std::getline(graph_file, line)) {
      std::cerr << "Unexpected error at line: " << i + 1 << ".\n";
      std::cerr << "Line content: " << line << std::endl;
      exit(EXIT_FAILURE);
    }
    // Do nothing. We don't need any information from the nodes
  }

  // Read all edges
  edges.reserve(edge_count);
  node_offsets.reserve(node_count + 1);

  for (size_t i = 0; i < edge_count; ++i) {
    if (!std::getline(graph_file, line)) {
      std::cerr << "Unexpected error at line: " << i + 1 << ".\n";
      std::cerr << "Line content: " << line << std::endl;
      exit(EXIT_FAILURE);
    }

    parse_edges(line, edges, reverse);
  }

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    std::cerr << "Error. There are still remaining lines in the file."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  graph_file.close();

  // Sort the edges and create the offset array
  std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
    if (a.src_id == b.src_id) {
      return a.trg_id < b.trg_id;
    }
    return a.src_id < b.src_id;
  });

  create_offset_array(edges, node_offsets);
  node_offsets.push_back(edge_count);

  return {std::move(node_offsets), std::move(edges)};
}

void Graph_parser::serialize(const std::string &file_path, const Graph &graph) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  // Write node_offsets size then data
  size_t node_offsets_size = graph.node_offsets.size();
  output_file_stream.write(reinterpret_cast<const char *>(&node_offsets_size),
                           sizeof(node_offsets_size));
  output_file_stream.write(
      reinterpret_cast<const char *>(graph.node_offsets.data()),
      node_offsets_size * sizeof(uint32_t));

  // Write edges size then data
  size_t edges_size = graph.edges.size();
  output_file_stream.write(reinterpret_cast<const char *>(&edges_size),
                           sizeof(edges_size));
  output_file_stream.write(reinterpret_cast<const char *>(graph.edges.data()),
                           edges_size * sizeof(Edge));

  output_file_stream.close();
}

Graph Graph_parser::deserialize(const std::string &file_path) {
  std::vector<uint32_t> node_offsets;
  std::vector<Edge> edges;

  std::ifstream input_file_stream(file_path, std::ios::binary);

  if (!input_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for reading."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read node_offsets size then data
  size_t node_offsets_size;
  input_file_stream.read(reinterpret_cast<char *>(&node_offsets_size),
                         sizeof(node_offsets_size));
  node_offsets.resize(node_offsets_size);
  input_file_stream.read(reinterpret_cast<char *>(node_offsets.data()),
                         node_offsets_size * sizeof(uint32_t));

  // Read edges size then data
  size_t edges_size;
  input_file_stream.read(reinterpret_cast<char *>(&edges_size),
                         sizeof(edges_size));
  edges.resize(edges_size);
  input_file_stream.read(reinterpret_cast<char *>(edges.data()),
                         edges_size * sizeof(Edge));

  input_file_stream.close();

  return {std::move(node_offsets), std::move(edges)};
}
