#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

struct BenchmarkParserResult {
  bool success;
  std::optional<std::string> error_message;
  std::optional<std::vector<std::pair<int, int>>> route_requests;
};

class Benchmark {
public:
  BenchmarkParserResult parse(const std::string &file_path);
  void run(const std::string &file_path,
           const std::vector<std::pair<int, int>> &route_requests);
};
