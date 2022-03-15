// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_NODEINDEX_H_
#define SRC_UTIL_INDEX_NODEINDEX_H_

#include <vector>
#include <cstdint>
#include "util/osm/Node.h"

namespace util {
namespace index {

using util::osm::Node;

/*
 * Sparse index that maps node ids to the nodes location and elevation.
 */ 
class NodeIndex {
 public:
  explicit NodeIndex(const uint64_t nodeCount);

  // Add a node to the index.
  void setNode(const uint64_t id, const double lon,
               const double lat, const int16_t elevation);

  // Update the elevation of the node.
  void updateElevation(const uint64_t id, const int16_t elevation);

  // Get the node to access its information.
  const Node& getNode(const uint64_t id) const;

  // After all nodes have been added, sort the index by node id for
  // fast lookup with binary search.
  void sort();

  // Clear the node index to free memory.
  void clear();

 private:
  // The index is saved as a vector that will be sorted
  // by node id for fast lookup.
  std::vector<Node> _nodeIndex;

  // Perform binary search to retrieve
  // the node's index in the vector.
  uint64_t binarySearchNodeIndex(const uint64_t id) const;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_NODEINDEX_H_
