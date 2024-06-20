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
  int number_of_nodes = dijkstra_forward.node_offsets_.size() - 1;

  std::random_device seed;
  std::mt19937 rng(seed());
  std::uniform_int_distribution<> distribution(0, number_of_nodes);

  // Landmarks for MV.fmi MV.que
  /*std::vector<int> landmarks({331304, 636957, 578588, 458565, 523923, 529314,*/
  /*                            272431, 302229, 614796, 537671, 286706, 130998,*/
  /*                            425843, 78952, 129243, 287420});*/
  // Landmarks for stgtregbz.fmi stgtregbz.que
  /*std::vector<int> landmarks({939825, 696456, 971661, 104348, 408060, 224980,*/
  /*                            1054703, 800484, 691, 418054, 26322, 712006,*/
  /*                            571912, 812992, 489153, 922435});*/

  for (int i = 0; i < number_of_landmarks; i++) {
    int random_landmark = distribution(rng);
    /*int random_landmark = landmarks[i];*/
    std::vector<int> distances_forward(number_of_nodes,
                                       std::numeric_limits<int>::max());
    std::vector<int> distances_reverse(number_of_nodes,
                                       std::numeric_limits<int>::max());

    dijkstra_forward.src_to_all(random_landmark, distances_forward);
    landmark_distances_forward[random_landmark] = distances_forward;

    dijkstra_reverse.src_to_all(random_landmark, distances_reverse);
    landmark_distances_reverse[random_landmark] = distances_reverse;

    /*std::cout << "Landmark: " << random_landmark << std::endl;*/
  }
}
