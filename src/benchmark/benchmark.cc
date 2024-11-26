#include "benchmark/benchmark.h"

#include <argparse/argparse.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "algorithms/alt.h"
#include "algorithms/dijkstra.h"
#include "algorithms/modified-alt.h"
#include "benchmark/bench_utils.h"
#include "constants.h"
#include "graph/graph.h"
#include "graph/graph_parser.h"
#include "graph/graph_partitioner.h"
#include "preprocessing/data_types.h"
#include "preprocessing/distance_calculation.h"
#include "preprocessing/farthest.h"
#include "preprocessing/landmark_selection.h"
#include "preprocessing/ldv_serializer.h"
#include "preprocessing/modified_alt_data_serializer.h"

namespace modified_alt {
namespace benchmark {

using Clock = std::chrono::high_resolution_clock;
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using algorithms::Dijkstra;
using std::chrono::duration_cast;

void run(int argc, char *argv[]) {
  using namespace preprocessing;

  argparse::ArgumentParser program("modified-ALT");

  program.add_argument("graph").help("the graph which should be used");
  program.add_argument("-a").help(
      "the algorithm which should be used (see below)");
  program.add_argument("-r")
      .help(
          "generate random route request with a solution for the given amount")
      .scan<'u', uint32_t>();
  program.add_argument("-p").help("preprocess the ALT algorithm").flag();
  program.add_argument("-P")
      .help("preprocess the modified-ALT algorithm")
      .flag();

  program.add_argument("--random")
      .help("use random landmark selection")
      .default_value(0)
      .scan<'u', uint32_t>();
  program.add_argument("--farthest")
      .help("use farthest landmark selection")
      .default_value(0)
      .scan<'u', size_t>();
  program.add_argument("--partition-corners")
      .help("use custom partition-corners landmark selection")
      .default_value(0)
      .scan<'u', uint32_t>();

  program.add_argument("-t").default_value(0).scan<'u', uint32_t>().help("WIP");
  program.add_argument("-T").flag().help("WIP");

  program.add_epilog(
      "The options for the -a flag are the "
      "following:\n\t\"dijkstra\"\n\t\"alt\"\n\t\"modified-alt\"");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    exit(1);
  }

  std::string selected_graph{program.get("graph")};
  std::string graph_path{GRAPH_DIR + selected_graph + ".fmi"};
  std::string query_path{BENCH_FILES_DIR + selected_graph + ".que"};
  std::string random_preprocessing_path{PREPROCESSING_DIR + selected_graph +
                                        "_random_modified-alt_data.bin"};
  std::string partition_corners_path{
      PREPROCESSING_DIR + selected_graph +
      "_partition-corners_modified-alt_data.bin"};
  std::string graph_partition_path{PARTITION_DIR + selected_graph};

  std::cout << "Starting parsing the graph " << selected_graph << " ...\n";
  graph::Graph graph{graph::ParseGraphFile(graph_path)};
  graph::Graph graph_reverse{graph::ParseGraphFile(graph_path, true)};
  std::cout << "Finished parsing the graph!\n";

  if (program.is_used("-r")) {
    std::cout << std::endl;
    GenerateBenchFiles(selected_graph, graph, program.get<uint32_t>("-r"));
    exit(EXIT_SUCCESS);
  }

  if (program.is_used("-a")) {
    std::string selected_algorithm{program.get("-a")};

    const std::vector<Query> route_requests{ParseRouteRequestFile(query_path)};

    if (selected_algorithm == "dijkstra") {
      BenchDijkstra(graph, route_requests);
    } else if (selected_algorithm == "alt" && program.is_used("--random")) {
      auto ldv{DeserializeLandmarkDistanceVectors(PREPROCESSING_DIR +
                                                  selected_graph + "_ldv.bin")};

      BenchAlt(graph, ldv, route_requests);
    } else if (selected_algorithm == "alt" && program.is_used("--farthest")) {
      auto ldv{DeserializeLandmarkDistanceVectors(
          PREPROCESSING_DIR + selected_graph + "_farthest.bin")};
      BenchAlt(graph, ldv, route_requests);
    } else if (selected_algorithm == "alt" &&
               program.is_used("--partition-corners")) {
      auto ldv{DeserializeLandmarkDistanceVectors(PREPROCESSING_DIR +
                                                  selected_graph + "_pc.bin")};
      BenchAlt(graph, ldv, route_requests);
    } else if (selected_algorithm == "modified-alt" &&
               program.is_used("--random")) {
      ModifiedAltData modified_alt_data{
          DeserializeModifiedAltData(random_preprocessing_path)};
      // auto landmarks{preprocessing::RandomLandmarkSelection(graph_reverse,
      // 724)}; auto
      // modified_alt_data{preprocessing::CalculateModifiedAltData(graph,
      // graph_reverse, landmarks)};

      BenchModifiedAlt(graph, modified_alt_data, route_requests);
    } else if (selected_algorithm == "modified-alt" &&
               program.is_used("--partition-corners")) {
      ModifiedAltData modified_alt_data{
          DeserializeModifiedAltData(partition_corners_path)};

      BenchModifiedAlt(graph, modified_alt_data, route_requests);
    } else {
      std::cerr << "\nYour input (\"" << selected_graph
                << "\") did not match any algorithm." << std::endl;
      std::cerr << "Check --help for more information." << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  if (program.is_used("-t")) {
    uint32_t cell_count{program.get<uint32_t>("-t")};
    auto start{Clock::now()};
    std::vector<graph::Cell> cells{graph::TranslateFile(
        cell_count, graph.GetVertices(),
        graph_partition_path + std::to_string(cell_count) + ".txt")};
    std::vector<uint32_t> landmarks{PartitionCornersLandmarkSelection(cells)};

    // auto ldv{CalculateLandmarkDistanceVectors(graph_reverse, landmarks)};
    ModifiedAltData modified_alt_data{
        CalculateModifiedAltData(graph, graph_reverse, landmarks)};

    auto end{Clock::now()};
    auto total_ms{duration_cast<Milliseconds>(end - start).count()};
    auto mins{total_ms / (60 * 1000)};
    total_ms %= (60 * 1000);
    auto secs{total_ms / 1000};
    auto ms{total_ms % 1000};
    std::cout << std::setfill('0') << std::setw(2) << mins << " m "
              << std::setw(2) << secs << " s " << std::setw(2) << ms << '\n';

    // SerializeLandmarkDistanceVectors(
    //     PREPROCESSING_DIR + selected_graph + "_test.bin", ldv);
    SerializeModifiedAltData(PREPROCESSING_DIR + selected_graph + "_test.bin",
                             modified_alt_data);

    std::cout << "Landmark count: " << landmarks.size() << '\n';

    auto geojson{graph::ConvertToGeoJSON(graph.GetVertices(), landmarks)};
    std::ofstream out("output-kahip.geojson");
    if (out) {
      out << geojson;
      out.close();
    }

    exit(EXIT_SUCCESS);
  }

  if (program.is_used("-T")) {
    const std::vector<Query> route_requests{ParseRouteRequestFile(query_path)};
    // auto ldv{DeserializeLandmarkDistanceVectors(PREPROCESSING_DIR +
    //                                             selected_graph +
    //                                             "_test.bin")};
    // BenchAlt(graph, ldv, route_requests);
    ModifiedAltData modified_alt_data{DeserializeModifiedAltData(
        PREPROCESSING_DIR + selected_graph + "_test.bin")};

    BenchModifiedAlt(graph, modified_alt_data, route_requests);
  }

  if (program.is_used("-p") && program.is_used("--random")) {
    uint32_t landmark_count{program.get<uint32_t>("--random")};

    auto start{Clock::now()};
    auto landmarks{RandomLandmarkSelection(graph_reverse, landmark_count)};
    auto landmark_distance_vectors{
        CalculateLandmarkDistanceVectors(graph_reverse, landmarks)};
    auto end{Clock::now()};
    auto duration{duration_cast<Microseconds>(end - start)};
    auto total_seconds{duration_cast<Seconds>(duration).count()};
    auto total_milliseconds{duration_cast<Milliseconds>(duration).count() %
                            1000};
    auto total_microseconds{duration.count() % 1000};

    std::cout << std::setfill('0') << std::setw(2) << total_seconds << " s "
              << std::setfill('0') << std::setw(3) << total_milliseconds
              << " ms " << std::setfill('0') << std::setw(3)
              << total_microseconds << " μs\n";

    SerializeLandmarkDistanceVectors(
        PREPROCESSING_DIR + selected_graph + "_ldv.bin",
        landmark_distance_vectors);

    auto geojson{graph::ConvertToGeoJSON(graph.GetVertices(), landmarks)};
    std::ofstream out("output-random.geojson");
    if (out) {
      out << geojson;
      out.close();
    }

    exit(EXIT_SUCCESS);
  } else if (program.is_used("-p") && program.is_used("--farthest")) {
    size_t landmark_count{program.get<size_t>("--farthest")};

    auto start{Clock::now()};
    auto landmark_distance_vectors{
        SelectFarthestLandmarks(graph, graph_reverse, landmark_count)};
    auto end{Clock::now()};
    auto duration{duration_cast<Microseconds>(end - start)};
    auto total_seconds{duration_cast<Seconds>(duration).count()};
    auto total_milliseconds{duration_cast<Milliseconds>(duration).count() %
                            1000};
    auto total_microseconds{duration.count() % 1000};

    std::cout << std::setfill('0') << std::setw(2) << total_seconds << " s "
              << std::setfill('0') << std::setw(3) << total_milliseconds
              << " ms " << std::setfill('0') << std::setw(3)
              << total_microseconds << " μs\n";

    SerializeLandmarkDistanceVectors(
        PREPROCESSING_DIR + selected_graph + "_farthest.bin",
        landmark_distance_vectors);

    exit(EXIT_SUCCESS);
  } else if (program.is_used("-p") && program.is_used("--partition-corners")) {
    uint32_t cell_count{program.get<uint32_t>("--partition-corners")};
    const std::vector<graph::Cell> cells{
        graph::PartitionGraphInCells(graph, cell_count)};
    std::vector<uint32_t> landmarks{PartitionCornersLandmarkSelection(cells)};
    std::cout << "Landmark count: " << landmarks.size() << '\n';

    auto ldv{CalculateLandmarkDistanceVectors(graph_reverse, landmarks)};
    SerializeLandmarkDistanceVectors(
        PREPROCESSING_DIR + selected_graph + "_pc.bin", ldv);

    auto geojson{graph::ConvertToGeoJSON(graph.GetVertices(), landmarks)};
    std::ofstream out("output-pc.geojson");
    if (out) {
      out << geojson;
      out.close();
    }

    exit(EXIT_SUCCESS);
  }

  if (program.is_used("-P") && program.is_used("--random")) {
    uint32_t landmark_count{program.get<uint32_t>("--random")};

    auto start{Clock::now()};
    std::vector<uint32_t> landmarks{
        RandomLandmarkSelection(graph_reverse, landmark_count)};

    auto geojson{graph::ConvertToGeoJSON(graph.GetVertices(), landmarks)};
    std::ofstream out("output-random.geojson");
    if (out) {
      out << geojson;
      out.close();
    }

    ModifiedAltData modified_alt_data{
        CalculateModifiedAltData(graph, graph_reverse, landmarks)};
    auto end{Clock::now()};
    auto total_ms{duration_cast<Milliseconds>(end - start).count()};
    auto mins{total_ms / (60 * 1000)};
    total_ms %= (60 * 1000);
    auto secs{total_ms / 1000};
    auto ms{total_ms % 1000};
    std::cout << std::setfill('0') << std::setw(2) << mins << " m "
              << std::setw(2) << secs << " s " << std::setw(2) << ms << '\n';

    SerializeModifiedAltData(random_preprocessing_path, modified_alt_data);

    exit(EXIT_SUCCESS);
  } else if (program.is_used("-P") && program.is_used("--partition-corners")) {
    uint32_t cell_count{program.get<uint32_t>("--partition-corners")};
    const std::vector<graph::Cell> cells{
        graph::PartitionGraphInCells(graph, cell_count)};
    std::vector<uint32_t> landmarks{PartitionCornersLandmarkSelection(cells)};
    std::cout << "Landmark count: " << landmarks.size() << '\n';

    auto geojson{graph::ConvertToGeoJSON(graph.GetVertices(), landmarks)};
    std::ofstream out("output-pc.geojson");
    if (out) {
      out << geojson;
      out.close();
    }

    ModifiedAltData modified_alt_data{
        CalculateModifiedAltData(graph, graph_reverse, landmarks)};

    SerializeModifiedAltData(partition_corners_path, modified_alt_data);

    exit(EXIT_SUCCESS);
  } else if (program.is_used("-P")) {
    std::cerr << "Please provide a landmark selection method." << std::endl;
    exit(EXIT_FAILURE);
  }
}

void BenchDijkstra(const graph::Graph &graph,
                   const std::vector<Query> &route_requests) {
  PrintBenchmarkHeader("Dijkstra");

  Dijkstra dijkstra(graph);
  std::vector<uint32_t> path_costs{};
  path_costs.reserve(graph.GetNumberOfNodes());
  Microseconds time{0};
  uint64_t search_space{0};

  uint32_t i{0};
  for (const Query &query : route_requests) {
    uint32_t nodes_checked{0};

    auto start{Clock::now()};
    uint32_t cost{dijkstra.ComputeSPSP(query.from, query.to, nodes_checked)};
    auto end{Clock::now()};
    auto duration{duration_cast<Microseconds>(end - start)};

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;
    ++i;

    path_costs.push_back(cost);
    search_space += nodes_checked;
    time += duration;
  }
  std::cout << std::endl;

  PrintBenchmarkResults("Dijkstra", time, search_space, route_requests.size());

  WriteCosts(RESULTS_DIR + "route_costs_dijkstra.txt", path_costs);
}

void BenchAlt(const graph::Graph &graph,
              preprocessing::LandmarkDistanceVectors &landmark_distance_vectors,
              const std::vector<Query> &route_requests) {
  PrintBenchmarkHeader("ALT");

  algorithms::ALT alt(graph, landmark_distance_vectors);
  std::vector<uint32_t> path_costs{};
  path_costs.reserve(graph.GetNumberOfNodes());
  Microseconds time{0};
  uint64_t search_space{0};

  uint32_t i{0};
  for (const Query &query : route_requests) {
    uint32_t nodes_checked{0};
    uint32_t from{query.from};
    uint32_t to{query.to};

    auto start{Clock::now()};
    // Sort landmark distance vectors based on the maximum lower bounds.
    alt.SortLandmarkDistanceVectors(from, to);
    uint32_t cost{alt.ComputeSPSP(from, to, 4, nodes_checked)};
    // uint32_t cost{alt.ComputeSPSP(from, to, landmark_distance_vectors.size(),
    // nodes_checked)};
    auto end{Clock::now()};
    auto duration{duration_cast<Microseconds>(end - start)};

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;
    ++i;

    path_costs.push_back(cost);
    search_space += nodes_checked;
    time += duration;
  }
  std::cout << std::endl;

  PrintBenchmarkResults("ALT", time, search_space, route_requests.size());

  WriteCosts(RESULTS_DIR + "route_costs_alt.txt", path_costs);
}

void BenchModifiedAlt(const graph::Graph &graph,
                      const preprocessing::ModifiedAltData &modified_alt_data,
                      const std::vector<Query> &route_requests) {
  PrintBenchmarkHeader("modified-ALT");

  algorithms::ModifiedAlt modified_alt(graph, modified_alt_data);
  std::vector<uint32_t> path_costs{};
  path_costs.reserve(graph.GetNumberOfNodes());
  Microseconds time{0};
  uint64_t search_space{0};

  uint32_t i{0};
  for (const Query &query : route_requests) {
    uint32_t nodes_checked{0};

    auto start{Clock::now()};
    uint32_t cost{
        modified_alt.ComputeSPSP(query.from, query.to, nodes_checked)};
    auto end{Clock::now()};
    auto duration{duration_cast<Microseconds>(end - start)};

    uint32_t progress =
        static_cast<uint32_t>((i + 1) * 100 / route_requests.size());
    std::cout << "\rProgress: " << progress << "%" << std::flush;
    ++i;

    path_costs.push_back(cost);
    search_space += nodes_checked;
    time += duration;
  }
  std::cout << std::endl;

  PrintBenchmarkResults("Modified-ALT", time, search_space,
                        route_requests.size());

  WriteCosts(RESULTS_DIR + "route_costs_modified-alt.txt", path_costs);
}

void PrintBenchmarkHeader(const std::string &algorithm) {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Starting " << algorithm << " benchmark." << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

void PrintBenchmarkResults(const std::string &algorithm,
                           const Microseconds time, const uint64_t search_space,
                           const size_t query_count) {
  auto divided_time{time / query_count};
  auto total_seconds{duration_cast<Seconds>(divided_time).count()};
  auto total_milliseconds{duration_cast<Milliseconds>(divided_time).count() %
                          1000};
  auto total_microseconds{divided_time.count() % 1000};

  std::cout << "----------------------------------------\n";
  std::cout << algorithm << " average execution time: " << std::setfill('0')
            << std::setw(2) << total_seconds << " s " << std::setfill('0')
            << std::setw(3) << total_milliseconds << " ms " << std::setfill('0')
            << std::setw(3) << total_microseconds << " μs\n";
  std::cout << "----------------------------------------\n";
  std::cout << algorithm
            << " average search space: " << search_space / query_count << '\n';
  std::cout << "----------------------------------------\n";
}

}  // namespace benchmark
}  // namespace modified_alt
