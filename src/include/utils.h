#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace modified_alt {
void PrintVector(const std::vector<uint32_t> &vec) {
  for (const auto &e : vec) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;
}
}  // namespace modified_alt
