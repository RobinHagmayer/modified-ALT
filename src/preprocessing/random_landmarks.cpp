#include "random_landmarks.h"

#include <limits>
#include <random>
#include <unordered_map>

// Graph must be in reverse
void Random_landmarks::preprocess(
    const int &number_of_landmarks, Dijkstra &dijkstra_forward,
    Dijkstra &dijkstra_reverse,
    std::unordered_map<int, std::vector<int>> &landmark_distances_forward,
    std::unordered_map<int, std::vector<int>> &landmark_distances_reverse) {
  std::cout << "Starting preprocessing..." << std::endl;
  int number_of_nodes = dijkstra_forward.node_offsets_.size() - 1;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  for (int i = 0; i < number_of_landmarks; i++) {
    int random_landmark = distribution(rng);
    /*std::vector<int> distances_forward(number_of_nodes,*/
    /*                                   std::numeric_limits<int>::max());*/
    std::vector<int> distances_reverse(number_of_nodes,
                                       std::numeric_limits<int>::max());

    /*dijkstra_forward.src_to_all(random_landmark, distances_forward);*/
    /*landmark_distances_forward[random_landmark] = distances_forward;*/

    dijkstra_reverse.src_to_all(random_landmark, distances_reverse);
    landmark_distances_reverse[random_landmark] = distances_reverse;

    std::cout << "Preprocessing finished!" << std::endl;
  }
}
