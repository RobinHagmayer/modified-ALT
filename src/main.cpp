#include "../include/GraphParser.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <graph_file_name>" << endl;
    return EXIT_FAILURE;
  }

  const std::string file_path = std::string(RESOURCE_PATH) + "/" + argv[1];

  // Check if graph file exists
  std::ifstream file(file_path);
  if (!file) {
    std::cerr << "Error: File " << file_path << " not found." << endl;
    return EXIT_FAILURE;
  }
  file.close();

  GraphParser graph_parser = GraphParser();

  auto start = std::chrono::high_resolution_clock::now();
  /*graph.parse_graph_file(file_path, true);*/
  GraphParserResult graph_result = graph_parser.parse(file_path);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  cout << "----------------------------------------------------------" << endl;
  cout << "Parsing of graph execution time: " << duration.count() << "ms."
       << endl;

  cout << "Number of nodes: " << graph_result.number_of_nodes << endl;
  cout << "Number of edges: " << graph_result.number_of_edges << endl;
  cout << "----------------------------------------------------------" << endl;
}
