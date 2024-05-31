#include <iostream>
#include <string>
#include "../include/Graph.h"

using std::cout;
using std::endl;

int main() {
  std::string file_path = std::string(RESOURCE_PATH) + "/toy.fmi";

  Graph toy = Graph();
  toy.parse_graph_file(file_path);

  cout << "Number of nodes: " << toy.number_of_nodes_ << endl;
  cout << "Number of edges: " << toy.number_of_edges_ << endl;
  cout << "----------------------------------------------------------" << endl;

  cout << "Everything went fine." << endl;
}
