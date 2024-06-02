#include "../include/GraphParser.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

GraphParserResult GraphParser::parse(const std::string &file_path,
                                     std::vector<int> &node_offsets,
                                     std::vector<Edge> &edges, bool verbose) {
  using std::cout;
  using std::endl;
  using std::nullopt;
  using std::string;
  using std::vector;

  // Declarations and initializations of variables
  GraphParserResult graph_parser_result = {false, nullopt, nullopt, nullopt};
  string line;
  int number_of_nodes;
  int number_of_edges;
  int curr_node = 0;
  int edge_counter = 0;

  // Open input file stream
  std::ifstream graph_file(file_path);

  if (!graph_file.is_open()) {
    graph_parser_result.error_message =
        "Error opening the file: " + file_path + ".";
    return graph_parser_result;
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
    graph_parser_result.error_message = "Error reading the number of nodes.";
    return graph_parser_result;
  }

  // Move to next line
  if (!std::getline(graph_file, line)) {
    graph_parser_result.error_message = "Unexpected error.";
    return graph_parser_result;
  }

  // Read number of edges
  std::istringstream number_of_edges_stream(line);
  if (!(number_of_edges_stream >> number_of_edges)) {
    graph_parser_result.error_message = "Error reading the number of edges.";
    return graph_parser_result;
  }

  // Read all nodes
  for (int i = 0; i < number_of_nodes; i++) {
    if (!std::getline(graph_file, line)) {
      graph_parser_result.error_message = "Unexpected error.";
      return graph_parser_result;
    }
    // Do nothing. We don't need any information from the nodes
  }

  // Read all edges
  edges.reserve(number_of_edges);
  node_offsets.reserve(number_of_nodes + 1);
  node_offsets.push_back(0);

  for (int i = 0; i < number_of_edges; i++) {
    if (!std::getline(graph_file, line)) {
      graph_parser_result.error_message = "Unexpected error.";
      return graph_parser_result;
    }
    // TODO: parse edges
    parse_edges(line, curr_node, edge_counter, node_offsets, edges);
  }
  node_offsets.push_back(number_of_edges);

  // There should be no more lines at this point
  if (std::getline(graph_file, line)) {
    graph_parser_result.error_message =
        "Error. There are still remaining lines in the file.";
    return graph_parser_result;
  }

  graph_file.close();

  graph_parser_result.success = true;
  graph_parser_result.number_of_nodes = number_of_nodes;
  graph_parser_result.number_of_edges = number_of_edges;
  return graph_parser_result;
}

void GraphParser::parse_edges(const std::string &line, int &curr_node,
                              int &edge_counter, std::vector<int> &node_offsets,
                              std::vector<Edge> &edges) {
  // Get src_id, trg_id and weight from an input string stream
  std::istringstream line_stream(line);
  int src_id, trg_id, weight;
  line_stream >> src_id >> trg_id >> weight;

  // Create an edge from the data and store it in the edges_ vector
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
  edge_counter++;
}
