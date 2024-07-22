#include "cli.h"
#include "ALT.h"
#include "dijkstra.h"
#include "random_landmarks.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <thread>

void CLI::print_menu() {
  using std::cout;
  using std::endl;

  cout << "\nMenu for the modified ALT approach for shortest path routing."
       << endl;
  cout << "  1: One to all request" << endl;
  cout << "  2: Source to target request" << endl;
  cout << "  3: ALT" << endl;
  cout << "  0: Quit the program" << endl;
  cout << "\nPlease enter your choice: " << endl;
}

void CLI::handle_one_to_all(Dijkstra &dijkstra) {
  int src_id;

  std::cout << "\nPlease enter the source node id: " << std::endl;
  std::cin >> src_id;

  int number_of_nodes = dijkstra.node_offsets.size() - 1;
  std::vector<int> distances(number_of_nodes, std::numeric_limits<int>::max());

  auto start = std::chrono::high_resolution_clock::now();
  /*dijkstra.src_to_all(src_id, distances);*/
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nDijkstra one to all execution time: " << duration.count()
            << "ms." << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
}

void CLI::handle_src_to_trg(Dijkstra &dijkstra) {
  int src_id;
  int trg_id;
  int nodes_checked = 0;

  std::cout << "\nPlease enter the source node id and the target node id. For "
               "example: 24 137"
            << std::endl;
  std::cin >> src_id >> trg_id;

  auto start = std::chrono::high_resolution_clock::now();
  /*int path_cost = dijkstra.src_to_trg(src_id, trg_id, nodes_checked);*/
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  /*std::cout << "\nCost: " << path_cost << std::endl;*/
  std::cout << "Dijkstra source to target execution time: " << duration.count()
            << "ms." << std::endl;
  std::cout << "Dijkstra search space: " << nodes_checked << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void handle_alt(const std::vector<int> &node_offsets,
                const std::vector<Edge> &edges,
                const std::vector<int> &node_offsets_rev,
                const std::vector<Edge> &edges_rev) {
  /*Dijkstra dijkstra_rev(node_offsets_rev, edges_rev);*/
  /*std::unordered_map<int, std::vector<int>> landmark_distances;*/

  /*Random_landmarks::preprocess(dijkstra_rev, 1, landmark_distances);*/

  int src_id;
  int trg_id;
  int nodes_checked = 0;

  std::cout << "\nPlease enter the source node id and the target node id. For "
               "example: 24 137"
            << std::endl;
  std::cin >> src_id >> trg_id;

  /*ALT alt(node_offsets, edges, landmark_distances);*/
  auto start = std::chrono::high_resolution_clock::now();
  /*int path_cost = alt.src_to_trg(src_id, trg_id, nodes_checked);*/
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  /*std::cout << "\nCost: " << path_cost << std::endl;*/
  std::cout << "ALT source to target execution time: " << duration.count()
            << "ms." << std::endl;
  std::cout << "ALT average search space: " << nodes_checked << std::endl;

  /*std::this_thread::sleep_for(std::chrono::milliseconds(3000));*/
  /*for (const auto &[key, value] : landmark_distances) {*/
  /*  std::cout << "Key: " << key << ", Value: " << &value << std::endl;*/
  /*}*/
}

void CLI::run(std::vector<int> &node_offsets, std::vector<Edge> &edges,
              std::vector<int> &node_offsets_rev,
              std::vector<Edge> &edges_rev) {
  using std::cin;
  using std::cout;
  using std::endl;

  /*Dijkstra dijkstra(node_offsets, edges);*/

  bool keep_running = true;
  int choice;

  while (keep_running) {
    print_menu();
    cin >> choice;

    switch (choice) {
    case 0:
      keep_running = false;
      break;
    case 1:
      /*handle_one_to_all(dijkstra);*/
      break;
    case 2:
      /*handle_src_to_trg(dijkstra);*/
      break;
    case 3:
      handle_alt(node_offsets, edges, node_offsets_rev, edges_rev);
      break;
    default:
      cout << "Invalid choice." << endl;
    }
  }

  cout << "\nExiting ..." << endl;
}
