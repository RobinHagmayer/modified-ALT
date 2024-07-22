#include "graph.h"

std::ostream &operator<<(std::ostream &os, const Edge &edge) {
  os << "Edge: (src_id: " << edge.src_id << ", trg_id: " << edge.trg_id
     << ", weight: " << edge.weight << ")";
  return os;
}
