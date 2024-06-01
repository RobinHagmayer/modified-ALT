#include "../include/Graph.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void Graph::parse_graph_file(const string &file_path, bool verbose) {
  // Open input file stream
  std::ifstream graph_file(file_path);

  if (!graph_file.is_open()) {
    cout << "Error opening the file: " << file_path << ". Exiting ..." << endl;
    exit(EXIT_FAILURE);
  }

  string line;

  // Ignore comments and empty lines
  while (std::getline(graph_file, line)) {
    if (!(line[0] == '#' || line.empty())) {
      break;
    }
  }

  // Read number of nodes
  std::istringstream number_of_nodes_stream(line);
  if (!(number_of_nodes_stream >> number_of_nodes_)) {
    cout << "Error reading number of nodes. Exiting ..." << endl;
    exit(EXIT_FAILURE);
  }

  // Move to next line
  if (!std::getline(graph_file, line)) {
    cout << "Unexpected error. Exiting ..." << endl;
    exit(EXIT_FAILURE);
  }

  // Read number of edges
  std::istringstream number_of_edges_stream(line);
  if (!(number_of_edges_stream >> number_of_edges_)) {
    cout << "Error reading number of edges. Exiting ..." << endl;
    exit(EXIT_FAILURE);
  }

  // Read all nodes
  for (int i = 0; i < number_of_nodes_; i++) {
    if (!std::getline(graph_file, line)) {
      cout << "Unexpected error. Exiting ..." << endl;
      exit(EXIT_FAILURE);
    }
    // Do nothing. We don't need any information from the nodes
  }

  // Read all edges
  int curr_node = 0;
  int edge_counter = 0;
  offset_for_node_.push_back(0);

  for (int i = 0; i < number_of_edges_; i++) {
    if (!std::getline(graph_file, line)) {
      cout << "Unexpected error. Exiting ..." << endl;
      exit(EXIT_FAILURE);
    }
    // TODO: parse edges
    parse_edges(line, curr_node, edge_counter);
  }
  offset_for_node_.push_back(number_of_edges_);

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    cout << "Error. There are still remaining lines in the file. Exiting ..."
         << endl;
    exit(EXIT_FAILURE);
  }

  graph_file.close();
}

void Graph::parse_edges(const string &line, int &curr_node, int &edge_counter) {
  // Get src_id, trg_id and weight from an input string stream
  std::istringstream line_stream(line);
  int src_id, trg_id, weight;
  line_stream >> src_id >> trg_id >> weight;

  // Create an edge from the data and store it in the edges_ vector
  Edge edge = {src_id, trg_id, weight};
  edges_.push_back(edge);

  // Create the offset for nodes vector

  if (curr_node + 1 == src_id) {
    offset_for_node_.push_back(edge_counter);
    curr_node++;
  } else if (curr_node + 1 < src_id) {
    int nodes_to_fill = src_id - curr_node;
    for (int i = 0; i < nodes_to_fill; i++) {
      offset_for_node_.push_back(edge_counter);
    }
    curr_node = src_id;
  }
  edge_counter++;
}

std::ostream &operator<<(std::ostream &os, const Edge &edge) {
  os << "Edge: (src_id: " << edge.src_id << ", trg_id: " << edge.trg_id
     << ", weight: " << edge.weight << ")";
  return os;
}

vector<int> Graph::get_offset_for_node() { return offset_for_node_; };

vector<Edge> Graph::get_edges() { return edges_; }
