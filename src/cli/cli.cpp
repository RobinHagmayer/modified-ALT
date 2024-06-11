#include "cli.h"
#include "dijkstra.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void CLI::print_menu() {
  using std::cout;
  using std::endl;

  cout << "\nMenu for the modified ALT approach for shortest path routing."
       << endl;
  cout << "  1: One to all request" << endl;
  cout << "  2: Source to target request" << endl;
  cout << "  0: Quit the program" << endl;
  cout << "\nPlease enter your choice: " << endl;
}

void CLI::handle_one_to_all(Dijkstra &dijkstra) {
  int src_id;

  std::cout << "\nPlease enter the source node id: " << std::endl;
  std::cin >> src_id;

  auto start = std::chrono::high_resolution_clock::now();
  DijkstraAllResult result = dijkstra.src_to_all(src_id);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  if (!result.success) {
    std::cerr << *result.error_message << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "\nDijkstra one to all execution time: " << duration.count()
            << "ms." << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
}

void CLI::handle_src_to_trg(Dijkstra &dijkstra) {
  int src_id;
  int trg_id;

  std::cout << "\nPlease enter the source node id and the target node id. For "
               "example: 24 137"
            << std::endl;
  std::cin >> src_id >> trg_id;

  auto start = std::chrono::high_resolution_clock::now();
  DijkstraSingleResult result = dijkstra.src_to_trg(src_id, trg_id);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  if (!result.success) {
    std::cerr << *result.error_message << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "\nCost: " << *result.cost << std::endl;
  std::cout << "Dijkstra source to target execution time: " << duration.count()
            << "ms." << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void CLI::run(std::vector<int> &node_offsets, std::vector<Edge> &edges) {
  using std::cin;
  using std::cout;
  using std::endl;

  Dijkstra dijkstra(node_offsets, edges);

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
      handle_one_to_all(dijkstra);
      break;
    case 2:
      handle_src_to_trg(dijkstra);
      break;
    default:
      cout << "Invalid choice." << endl;
    }
  }

  cout << "\nExiting ..." << endl;
}
