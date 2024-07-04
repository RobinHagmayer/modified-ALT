#pragma once

#include <iostream>

struct Edge {
  int src_id;
  int trg_id;
  int weight;

  Edge(int src_id, int trg_id, int weight)
      : src_id(src_id), trg_id(trg_id), weight(weight) {}

  Edge() : src_id(0), trg_id(0), weight(0) {}
};

std::ostream &operator<<(std::ostream &os, const Edge &edge);
