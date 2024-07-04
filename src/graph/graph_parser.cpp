#include "graph_parser.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Graph_parser::parse(const std::string &file_path,
                         std::vector<int> &node_offsets,
                         std::vector<Edge> &edges, bool reverse) {
  using std::cerr;
  using std::endl;

  // Declarations and initializations of variables
  std::string line;
  int number_of_nodes;
  int number_of_edges;

  // Open input file stream
  std::ifstream graph_file(file_path);

  if (!graph_file.is_open()) {
    cerr << "Error opening the file: " << file_path << "." << endl;
    exit(EXIT_FAILURE);
  }

  // Ignore comments and empty lines
  while (std::getline(graph_file, line)) {
    if (!(line[0] == '#' || line.empty())) {
      break;
    }
  }

  // Read number of nodes
  std::istringstream number_of_nodes_stream(line);
  if (!(number_of_nodes_stream >> number_of_nodes)) {
    cerr << "Error reading the number of nodes." << endl;
    exit(EXIT_FAILURE);
  }

  // Move to next line
  if (!std::getline(graph_file, line)) {
    cerr << "Unexpected error between reading number of nodes and number of "
            "edges."
         << endl;
    exit(EXIT_FAILURE);
  }

  // Read number of edges
  std::istringstream number_of_edges_stream(line);
  if (!(number_of_edges_stream >> number_of_edges)) {
    cerr << "Error reading the number of edges." << endl;
    exit(EXIT_FAILURE);
  }

  // Read all nodes
  for (int i = 0; i < number_of_nodes; i++) {
    if (!std::getline(graph_file, line)) {
      cerr << "Unexpected error at line: " << i + 1 << "." << endl;
      cerr << "Line content: " << line << endl;
      exit(EXIT_FAILURE);
    }
    // Do nothing. We don't need any information from the nodes
  }

  // Read all edges
  edges.reserve(number_of_edges);
  node_offsets.reserve(number_of_nodes + 1);

  for (int i = 0; i < number_of_edges; i++) {
    if (!std::getline(graph_file, line)) {
      cerr << "Unexpected error at line: " << i + 1 << "." << endl;
      cerr << "Line content: " << line << endl;
      exit(EXIT_FAILURE);
    }

    parse_edges(line, edges, reverse);
  }

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    cerr << "Error. There are still remaining lines in the file." << endl;
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
  node_offsets.push_back(number_of_edges);
}

void Graph_parser::parse_edges(const std::string &line,
                               std::vector<Edge> &edges, bool reverse) {
  std::istringstream line_stream(line);
  int src_id, trg_id, weight;

  if (reverse) {
    line_stream >> src_id >> trg_id >> weight;
    Edge edge(trg_id, src_id, weight);
    edges.push_back(edge);
  } else {
    line_stream >> src_id >> trg_id >> weight;
    Edge edge(src_id, trg_id, weight);
    edges.push_back(edge);
  }
}

void Graph_parser::create_offset_array(const std::vector<Edge> &edges,
                                       std::vector<int> &node_offsets) {
  int curr_node = edges[0].src_id;
  node_offsets.push_back(curr_node);

  for (int i = 0; i < edges.size(); i++) {
    int src_id = edges[i].src_id;
    int trg_id = edges[i].trg_id;

    if (curr_node + 1 == src_id) {
      node_offsets.push_back(i);
      curr_node++;
    } else if (curr_node + 1 < src_id) {
      int nodes_to_fill = src_id - curr_node;
      for (int j = 0; j < nodes_to_fill; j++) {
        node_offsets.push_back(i);
      }
      curr_node = src_id;
    }
  }
}

void Graph_parser::serialize(const std::string &file_path,
                             const std::vector<int> &node_offsets,
                             const std::vector<Edge> &edges) {
  std::ofstream output_file_stream(file_path, std::ios::binary);

  if (!output_file_stream) {
    std::cerr << "Error opening the file: " << file_path << " for writing."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  // Write node_offsets size then data
  size_t node_offsets_size = node_offsets.size();
  output_file_stream.write(reinterpret_cast<const char *>(&node_offsets_size),
                           sizeof(node_offsets_size));
  output_file_stream.write(reinterpret_cast<const char *>(node_offsets.data()),
                           node_offsets_size * sizeof(int));

  // Write edges size then data
  size_t edges_size = edges.size();
  output_file_stream.write(reinterpret_cast<const char *>(&edges_size),
                           sizeof(edges_size));
  output_file_stream.write(reinterpret_cast<const char *>(edges.data()),
                           edges_size * sizeof(Edge));

  output_file_stream.close();
}

void Graph_parser::deserialize(const std::string &file_path,
                               std::vector<int> &node_offsets,
                               std::vector<Edge> &edges) {
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
                         node_offsets_size * sizeof(int));

  // Read edges size then data
  size_t edges_size;
  input_file_stream.read(reinterpret_cast<char *>(&edges_size),
                         sizeof(edges_size));
  edges.resize(edges_size);
  input_file_stream.read(reinterpret_cast<char *>(edges.data()),
                         edges_size * sizeof(Edge));

  input_file_stream.close();
}
