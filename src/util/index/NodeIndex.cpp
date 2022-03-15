// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include <ctime>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <vector>
#include "util/osm/Node.h"
#include "util/index/NodeIndex.h"

using util::osm::Node;
using util::index::NodeIndex;

// ____________________________________________________________________________
NodeIndex::NodeIndex(const uint64_t nodeCount) {
  _nodeIndex.reserve(nodeCount);
}

// ____________________________________________________________________________
void NodeIndex::setNode(const uint64_t id, const double lon,
                        const double lat, const int16_t elevation) {
  _nodeIndex.emplace_back(id, lon, lat, elevation);
}

// ____________________________________________________________________________
void NodeIndex::updateElevation(const uint64_t id, const int16_t elevation) {
  const uint64_t index = binarySearchNodeIndex(id);
  _nodeIndex[index].elevation = elevation;
}

// ____________________________________________________________________________
const Node& NodeIndex::getNode(const uint64_t id) const {
  const uint64_t index = binarySearchNodeIndex(id);
  return _nodeIndex[index];
}

// ____________________________________________________________________________
uint64_t NodeIndex::binarySearchNodeIndex(const uint64_t id) const {
  // Binary search.
  uint64_t index = 0;
  uint64_t left = 0;
  uint64_t right = _nodeIndex.size();
  while (left <= right) {
    uint64_t m = floor((left + right) / 2);
    if (_nodeIndex[m].id < id) {
      left = m + 1;
    } else if (
      _nodeIndex[m].id > id) {
      right = m - 1;
    } else {
      index = m;
      break;
    }
  }
  return index;
}

// ____________________________________________________________________________
void NodeIndex::sort() {
  // Sort the vector by node ID.
  std::sort(_nodeIndex.begin(), _nodeIndex.end(),
            [](const Node& i, const Node& j) {
              return i.id <= j.id;
            });

  // Remove duplicates.
  _nodeIndex.erase(std::unique(_nodeIndex.begin(), _nodeIndex.end()),
                   _nodeIndex.end());
}

// ____________________________________________________________________________
void NodeIndex::clear() {
  _nodeIndex.clear();
}
