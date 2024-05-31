#include "../include/Graph.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void Graph::parse_graph_file(const string &file_path) {
  // Open input file stream
  std::ifstream graph_file(file_path);

  if (!graph_file.is_open()) {
    cout << "Error opening the file: " << file_path << ". Exiting ..." << endl;
    exit(0);
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
    exit(0);
  }

  // Move to next line
  if (!std::getline(graph_file, line)) {
    cout << "Unexpected error. Exiting ..." << endl;
    exit(0);
  }

  // Read number of edges
  std::istringstream number_of_edges_stream(line);
  if (!(number_of_edges_stream >> number_of_edges_)) {
    cout << "Error reading number of edges. Exiting ..." << endl;
    exit(0);
  }

  // Read all nodes
  for (int i = 0; i < number_of_nodes_; i++) {
    if (!std::getline(graph_file, line)) {
      cout << "Unexpected error. Exiting ..." << endl;
      exit(0);
    }
    // Do nothing. We don't need any information from the nodes
  }

  // Read all edges
  for (int i = 0; i < number_of_edges_; i++) {
    if (!std::getline(graph_file, line)) {
      cout << "Unexpected error. Exiting ..." << endl;
      exit(0);
    }
    // TODO: parse edges
  }

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    cout << "Error. There are still remaining lines in the file. Exiting ..."
         << endl;
    exit(0);
  }

  graph_file.close();
}

void Graph::parse_edges(const string &line) {
  // TODO: implement parse edges
}

vector<int> Graph::get_offset_for_node() { return offset_for_node_; };

vector<edge> Graph::get_edges() { return edges_; }
