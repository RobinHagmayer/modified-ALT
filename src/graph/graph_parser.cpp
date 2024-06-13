#include "graph_parser.h"

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
  int curr_node = 0;
  int edge_counter = 0;

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
  node_offsets.push_back(0);

  for (int i = 0; i < number_of_edges; i++) {
    if (!std::getline(graph_file, line)) {
      cerr << "Unexpected error at line: " << i + 1 << "." << endl;
      cerr << "Line content: " << line << endl;
      exit(EXIT_FAILURE);
    }

    parse_edges(line, curr_node, edge_counter, node_offsets, edges, reverse);
  }
  node_offsets.push_back(number_of_edges);

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    cerr << "Error. There are still remaining lines in the file." << endl;
    exit(EXIT_FAILURE);
  }

  graph_file.close();
}

void Graph_parser::parse_edges(const std::string &line, int &curr_node,
                               int &edge_counter,
                               std::vector<int> &node_offsets,
                               std::vector<Edge> &edges, bool reverse) {
  std::istringstream line_stream(line);
  int src_id, trg_id, weight;

  if (reverse) {
    line_stream >> trg_id >> src_id >> weight;
    Edge edge = {trg_id, src_id, weight};
    edges.push_back(edge);

    // Create the offset for nodes vector
    if (curr_node + 1 == trg_id) {
      node_offsets.push_back(edge_counter);
      curr_node++;
    } else if (curr_node + 1 < trg_id) {
      int nodes_to_fill = trg_id - curr_node;
      for (int i = 0; i < nodes_to_fill; i++) {
        node_offsets.push_back(edge_counter);
      }
      curr_node = trg_id;
    }
  } else {
    line_stream >> src_id >> trg_id >> weight;
    Edge edge = {src_id, trg_id, weight};
    edges.push_back(edge);

    // Create the offset for nodes vector
    if (curr_node + 1 == src_id) {
      node_offsets.push_back(edge_counter);
      curr_node++;
    } else if (curr_node + 1 < src_id) {
      int nodes_to_fill = src_id - curr_node;
      for (int i = 0; i < nodes_to_fill; i++) {
        node_offsets.push_back(edge_counter);
      }
      curr_node = src_id;
    }
  }

  edge_counter++;
}
