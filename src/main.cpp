#include "../include/GraphParser.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <graph_file_name>" << endl;
    return EXIT_FAILURE;
  }

  const std::string file_path = std::string(RESOURCE_PATH) + "/" + argv[1];
  std::vector<int> node_offsets;
  std::vector<Edge> edges;

  // Check if graph file exists
  std::ifstream file(file_path);
  if (!file) {
    cerr << "Error: File " << file_path << " not found." << endl;
    return EXIT_FAILURE;
  }
  file.close();

  GraphParser graph_parser = GraphParser();

  auto start = std::chrono::high_resolution_clock::now();
  GraphParserResult graph_parser_result =
      graph_parser.parse(file_path, node_offsets, edges);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  if (!graph_parser_result.success) {
    cerr << "Error while parsing the graph file." << endl;
    if (graph_parser_result.error_message.has_value()) {
      cerr << "Error message: " << *graph_parser_result.error_message << endl;
    }
    return EXIT_FAILURE;
  }

  cout << "----------------------------------------------------------" << endl;
  cout << "Parsing of graph execution time: " << duration.count() << "ms."
       << endl;

  cout << "Number of nodes: " << *graph_parser_result.number_of_nodes << endl;
  cout << "Number of edges: " << *graph_parser_result.number_of_edges << endl;
  cout << "----------------------------------------------------------" << endl;
}
