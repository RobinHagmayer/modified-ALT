#include "app.h"
#include "cli.h"
#include "graph.h"
#include "graph_parser.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string App::graph_dir = std::string(GRAPH_DIR);

void App::run(const int &argc, const char *const *argv) {
  App::Result validation_result = validate_cla(argc, argv);
  if (!validation_result.ok) {
    std::cerr << validation_result.message << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<int> node_offsets;
  std::vector<Edge> edges;
  std::vector<int> node_offsets_rev;
  std::vector<Edge> edges_rev;
  read_graph(graph_dir + argv[1], node_offsets, edges, node_offsets_rev,
             edges_rev);

  CLI cli;
  cli.run(node_offsets, edges, node_offsets_rev, edges_rev);
}

App::Result App::validate_cla(const int &argc, const char *const *argv) {
  if (argc != 2) {
    std::stringstream message_stream;
    message_stream << "Usage: " << argv[0] << " <graph_file_name>.";
    return {false, message_stream.str()};
  }

  std::ifstream file(graph_dir + argv[1]);
  if (!file) {
    return {false, "Graph file was not found in the 'data/graphs/' directory."};
  }
  file.close();

  return {true, "Validation of command line arguments was successful."};
}

void App::read_graph(const std::string &file_path,
                     std::vector<int> &node_offsets, std::vector<Edge> &edges,
                     std::vector<int> &node_offsets_rev,
                     std::vector<Edge> &edges_rev) {
  using Clock = std::chrono::high_resolution_clock;
  using Milliseconds = std::chrono::milliseconds;
  using std::cout;
  using std::endl;

  Graph_parser graph_parser;

  cout << "Starting reading in the graph..." << endl;
  auto start = Clock::now();
  /*graph_parser.parse(file_path, node_offsets, edges, false);*/
  auto end = Clock::now();
  auto duration = std::chrono::duration_cast<Milliseconds>(end - start);

  cout << "----------------------------------------------------------" << endl;
  cout << "Parsing of graph execution time: " << duration.count() << "ms."
       << endl;
  cout << "----------------------------------------------------------" << endl;

  auto start_parser_rev = Clock::now();
  /*graph_parser.parse(file_path, node_offsets_rev, edges_rev, true);*/
  auto end_parser_rev = Clock::now();
  auto duration_parser_rev = std::chrono::duration_cast<Milliseconds>(
      end_parser_rev - start_parser_rev);

  cout << "Parsing of reverse graph execution time: "
       << duration_parser_rev.count() << "ms." << endl;
  cout << "----------------------------------------------------------" << endl;
}
